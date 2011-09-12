#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# vim:set ts=2 sw=2 et:
#
# -----------------------------------------------------------------------
#
# Go2C 0.1
#
# Go2C is based on the c-to-c.py example from pycparser by Eli Bendersky,
# and uses pycparser extensively.
#
# Alexander Rødseth <rodseth@gmail.com>
# License: BSD
#
# -----------------------------------------------------------------------
#
#-----------------------------------------------------------------
# pycparser: c-to-c.py
#
# Example of a C code generator from pycparser AST nodes, serving 
# as a simplistic translator from C to AST and back to C.
# Note: at this stage, the example is "alpha release" and considered 
# experimental. Please file any bugs you find in the Issues page on pycparser's
# website.
#
# Copyright (C) 2008-2011, Eli Bendersky
# License: BSD
#-----------------------------------------------------------------

from __future__ import print_function
import tempfile
import sys

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
#
sys.path.insert(0, '..')

from pycparser import c_parser, c_ast, parse_file

REPLACEMENT_FUNCTIONS = {
  "stat": "syscall.Stat",
  "access": "syscall.Access",
  "rand": "rand.Float64"
}

REPLACEMENT_TYPES = {
  "static struct stat": "syscall.Stat_t",
  "char *": "CString",
  "char": "byte",
  "unsigned char": "byte",
  "int *": "*int",
  "int": "int",
  "unsigned int": "uint",
  "unsigned int *": "*uint",
  "void": "",
  "short": "int16",
  "short *": "*int16",
  "unsigned short": "uint16",
  "unsigned short *": "*uint16",
  "float": "float32",
  "float *": "*float32",
  "double": "float64",
  # TODO: check if int is int32 in Go
  "long": "int",
  # TODO: Needs a better plan for static
  "static long": "int",
  "static int": "int"
}

REPLACEMENT_MACROS = {
    "S_ISDIR" : ["syscall", "(((", ") & syscall.S_IFMT) == syscall.S_IFDIR)"]
}

REPLACEMENT_DEFS = {
    "F_OK" : 0,
    "X_OK" : 1,
    "W_OK" : 2,
    "R_OK" : 4
}

CUSTOM_FUNCTIONS = {
    "atoi":   ["strconv",  "func atoi(a string) int {\n\tv, _ := strconv.Atoi(a)\n\treturn v\n}"],
    "sleep":  ["time",     "func sleep(sec int64) {\n\ttime.Sleep(1e9 * sec)\n}"],
    "getchar":["fmt",      'func getchar() byte {\n\tvar b byte\n\tfmt.Scanf("%c", &b)\n\treturn b\n}'],
    "putchar":["fmt",      'func putchar(b byte) {\n\tfmt.Printf("%c", b)\n}'],
    "abs":    ["",         'func abs(a int) int {\n\tif a >= 0 {\n\t\treturn a\n\t}\n\treturn -a\n}'],
    "strcpy": ["CString",  "func strcpy(a *CString, b CString) {\n\ta = &b\n}"],
    "strcmp": ["CString",  'func strcmp(acs, bcs CString) int {\n\ta := acs.ToString()\n\tb := bcs.ToString()\n\tif a == b {\n\t\treturn 0\n\t}\n\talen := len(a)\n\tblen := len(b)\n\tminlen := blen\n\tif alen < minlen {\n\t\tminlen = alen\n\t}\n\tfor i := 0; i < minlen; i++ {\n\t\tif a[i] > b[i] {\n\t\t\treturn 1\n\t\t} else if a[i] < b[i] {\n\t\t\treturn -1\n\t\t}\n\t}\n\tif alen > blen {\n\t\treturn 1\n\t}\n\treturn -1\n}'],
    "strlen": ["CString",  'func strlen(c CString) int {\n\treturn len(c.ToString())\n}'],
    "printf": ["fmt", "func printf(format CString, a ...interface{}) {\n\tfmt.Printf(format.ToString(), a...)\n}"]
}

SKIP_INCLUDES = ["CString"]

WHOLE_PROGRAM_REPLACEMENTS = {
    r'fmt.Printf("\n")': "fmt.Println()",
    "func main(argc int, argv *[]string) int {": "func main() {\n\tflag.Parse()\n\targv := flag.Args()\n\targc := len(argv)+1\n",
    "argv[": "argv[-1+",
    "for (1)": "for",
    "\n\n\n": "\n",
    # TODO: Find a sensible way to figure out when a program wants strings and when it wants byte arrays
    "*[]byte": "CString",
    #"*[128]byte = new([128]byte)": "*string",
    #
    "'\\0'": "'\\x00'"
}

class GoGenerator(object):
    """ Uses the same visitor pattern as c_ast.NodeVisitor, but modified to
        return a value from each visit method, using string accumulation in 
        generic_visit.
    """
    def __init__(self):
        self.output = ''
        
        # Statements start with indentation of self.indent_level spaces, using
        # the _make_indent method
        #
        self.indent_level = 0
        self.imports = []
        # in main()
        self.inmain = False
        # scopeless declarations
        self.vartypes = {}
        # function name for the current function
        self.current_function_name = ""
        # some functions should return bool instead of int
        self.should_return_bool_instead_of_int = []
        # custom functions that has been used
        self.used_custom_functions = []
        # variables that has to be renamed
        self.renames = {}

    def _make_packagename(self):
      return "package main\n\n"

    def _make_imports(self):
      s = "import (\n"
      for imp in self.imports:
        if imp and (imp not in SKIP_INCLUDES):
          s += "  \"%s\"\n" % (imp)
      s += ")\n\n"
      return s

    def _make_customfunc(self):
      """Define custom functions"""
      s = ""
      if "CString" in self.imports:
        s += "type CString []byte\n\nfunc (c CString) ToString() string {\n\ts := \"\"\n\tfor _, e := range c {\n\t\tif e == 0 {\n\t\t\t\tbreak\n\t\t}\n\t\ts += string(e)\n\t}\n\treturn s\n}\n\nfunc NewCString(s string) CString {\n\tc := make(CString, len(s)+1)\n\tfor i, e := range s {\n\t\tc[i] = byte(e)\n\t}\n\t// The last byte will already be 0\n\treturn c\n}\n\n"
        del self.imports[self.imports.index("CString")]
      for fun in self.used_custom_functions:
        if fun in CUSTOM_FUNCTIONS:
          s += CUSTOM_FUNCTIONS[fun][1] + "\n\n"
      return s

    def make_header(self):
      return self._make_packagename() + self._make_imports() + self._make_customfunc()
    
    def _make_indent(self):
        return ' ' * self.indent_level
    
    def visit(self, node):
        """Called recursively, beware"""
        method = 'visit_' + node.__class__.__name__
        return getattr(self, method, self.generic_visit)(node)
    
    def generic_visit(self, node):
        #~ print('generic:', type(node))
        if node is None:
            return ''
        else:
            return ''.join(self.visit(c) for c in node.children())
    
    def visit_Constant(self, n):
        v = n.value
        if v.startswith("\"") and v.endswith("\""):
          return "NewCString(" + v + ")"
        return n.value
        
    def visit_ID(self, n):
        name = n.name
        if name in self.renames:
          return self.renames[name]
        return name

    def visit_ArrayRef(self, n):
        arrref = self._parenthesize_unless_simple(n.name)
        contents = self.visit(n.subscript)
        if "++" in contents or "--" in contents:
          if "++" in contents:
            op = "++"
          else:
            op = "--"
          # Insert markers to move the ++/-- expression down or up at a later stage
          contents = "$$$" + contents + "$$$"
        return arrref + '[' + contents + ']'

    def visit_StructRef(self, n):
        sref = self._parenthesize_unless_simple(n.name)
        return sref + n.type + self.visit(n.field)

    def visit_FuncCall(self, n):
        s = ""
        fref = self._parenthesize_unless_simple(n.name)
        # Use the replacement table to convert from C to Go functions
        if fref in REPLACEMENT_FUNCTIONS:
          gofref = REPLACEMENT_FUNCTIONS[fref]
          # ... and import the right package
          pkg = ".".join(gofref.split(".")[:-1])
          if pkg not in self.imports:
            self.imports.append(pkg)
          fref = gofref
        elif fref in CUSTOM_FUNCTIONS:
          if not fref in self.used_custom_functions:
            self.used_custom_functions.append(fref)
            pkg = CUSTOM_FUNCTIONS[fref][0]
            if pkg not in self.imports:
              self.imports.append(pkg)
            if "CString" in CUSTOM_FUNCTIONS[fref][1]:
              if "CString" not in self.imports:
                self.imports.append("CString")
          #log("WANTS TO ENABLE CUSTOM FUNCTION: " + fref)
        elif fref in REPLACEMENT_MACROS:
          pkg, first_part, last_part = REPLACEMENT_MACROS[fref]
          # ... and import the right package
          if pkg not in self.imports:
            self.imports.append(pkg)
          s = first_part + self.visit(n.args) + last_part
        if not s:
          s = fref + '(' + self.visit(n.args) + ')'
        for d in REPLACEMENT_DEFS:
          if d in s:
            s = s.replace(d, str(REPLACEMENT_DEFS[d]) + "/*" + d + "*/")
        return s

    def visit_UnaryOp(self, n):
        operand = self._parenthesize_unless_simple(n.expr)
        if n.op == 'p++':
            return '%s++' % operand
        elif n.op == 'p--':
            return '%s--' % operand
        elif n.op == 'sizeof':
            # Always parenthesize the argument of sizeof since it can be 
            # a name.
            if not "unsafe" in self.imports:
              self.imports.append("unsafe")
            return 'unsafe.Sizeof(%s)' % self.visit(n.expr)
        else:
            return '%s%s' % (n.op, operand)

    def visit_BinaryOp(self, n):
        lval_str = self._parenthesize_if(n.left, 
                            lambda d: not self._is_simple_node(d))
        rval_str = self._parenthesize_if(n.right, 
                            lambda d: not self._is_simple_node(d))
        return '%s %s %s' % (lval_str, n.op, rval_str)
    
    def visit_Assignment(self, n):
        rval_str = self._parenthesize_if(
                            n.rvalue, 
                            lambda n: isinstance(n, c_ast.Assignment))
        op = n.op
        if "=" in rval_str:
          # There is probably an assignment on the right side, not good
          r = rval_str.strip()
          if r.startswith("(") and r.endswith(")"):
            rval_str = rval_str.split("(", 1)[1].rsplit(")", 1)[0]
          # Convert a = (b = 1) to a, b = 1, 1
          # TODO: This covers some code, but not everything, make it more general
          if (op == "=") and ("=" in rval_str):
            op = ", "
            rval_str += "," + rval_str.split("=")[1]
        lvalue = self.visit(n.lvalue)
        if "[" in lvalue:
          name = lvalue.split("[")[0].strip()
          if name in self.vartypes:
            type = self.vartypes[name]
            log(name + " is " + type)
            if type in ["*string", "*[]byte"]:
              pos = lvalue.split("[")[1].split("]")[0]
              #lvalue = "*" + name
              #name = "(*" + name + ")"
              # This doesn't work
              #rvalue = name + "[:" + pos + "] + string(" + rval_str + ") + " + name + "[" + pos + "+1:]"
              #rval_str = rvalue
          #if name in self.vartypes:
          #  log(lvalue + " IS " + self.vartypes[lvalue] + "!!!")
          #  if self.vartypes[lvalue] == "*string":
          #    log("STRING! " + lvalue)
        return '%s %s %s' % (lvalue, op, rval_str)
    
    def visit_IdentifierType(self, n):
        return ' '.join(n.names)
    
    def visit_Decl(self, n, no_type=False):
        # no_type is used when a Decl is part of a DeclList, where the type is
        # explicitly only for the first delaration in a list.
        #
        s = n.name if no_type else self._generate_decl(n)

        add_addr_of = False
        found = False
        #log("from: " + s)
        arraytype = ""
        basetype = ""
        repl_keys = REPLACEMENT_TYPES.keys()
        for t in repl_keys:
          if s.startswith(t):
            twofirstwords = " ".join(s.split(" ", 2)[:2])
            if ("*" in twofirstwords) and ("*" not in t):
              # don't match "int" if it could be an "int *"
              #log("skipping: " + twofirstwords + " (of " + s + ")")
              continue
            if s.endswith("]"):
              arraytype = "[" + s.split("[", 1)[1]
              s = s.replace(arraytype, "")
              #log("array: " + arraytype)

            s = s.replace(t, "", 1)
            s += " " + REPLACEMENT_TYPES[t]
            l = s.rsplit(" ", 1)
            arraystar = ""
            if ("[" in arraytype) and ("]" in arraytype):
              arraystar = "*"
            s = l[0] + " " + arraystar + arraytype + l[1]
            found = True
            basetype = l[1]
            break

        #log("changed to: " + s + "\n")

        if not found:
          print("// C2GO: Unconverted declaration: " + s.strip() + "\n")
          return s.rstrip() + " // C2GO: ???"

        if "(" in s:
          # We can guess that this is a function
          s = "func " + s
          self.inmain = n.name == "main"
          self.current_function_name = n.name

        if "(void)" in s:
          s = s.replace("(void)", "()", 1)
        if "main() int" in s:
          s = s.replace("main() int", "main()")

        #print("in main:", self.inmain, n.name)

        if n.bitsize: s += ' : ' + self.visit(n.bitsize)
        if n.init:
            if isinstance(n.init, c_ast.ExprList):
                s += ' = {' + self.visit(n.init) + '}'
            else:
                s += ' = ' + self.visit(n.init)
         
        if not s.strip().startswith("func"):
          # Assume it's a variable declaration
          s = "var " + s
          s = s.replace("  ", " ")
          if "var long" in s:
            # There is no long double in Go (float128)
            s = s.replace("long", "", 1).replace("  ", " ")
          if "var static struct" in s:
            s = s.replace("var static struct", "struct", 1)
            s = "var " + s.split(" ")[-1] + " " + " ".join(s.split(" ")[:-1])
          if ("[" in s) and ("]" in s):
            arraynumber = s.split("[", 1)[1].rsplit("]", 1)[0]
            if arraynumber:
              #log("!!!" + s)
              if "=" not in s:
                s += " = new(" + arraytype + basetype + ")"
              elif ("=" in s) and ("{" in s):
                add_addr_of = True
          #log("DECLARING: " + s)
          if "=" in s:
            vartype = s.split("=")[0].strip().split(" ")[-1]
          else:
            vartype = s.split(" ")[-1]
          varname = s.split(" ")[1]
          if varname in ["len"]: # is varname a Go keyword?
            oldvarname = varname
            varname = varname + "_" + varname
            self.renames[oldvarname] = varname
            #log("RENAME FROM " + oldvarname + " TO " + varname)
            s = s.replace(oldvarname, varname)
          # Storing the type for later reference (by visit_Ternary, for example)
          self.vartypes[varname] = vartype
          #log(varname + " is of type: " + vartype)
        else:
          # Remove "var " from function declaration
          s = s.replace("var ", "").replace("  ", " ")
          # Remove "= new" from function declaration
          removefrom = " = new"
          removeto = ")" # including
          if removefrom in s:
            remove_n = s.count(removefrom)
            for n in range(remove_n):
              pos = s.find(removefrom)
              #log("Remove from =new to ) at pos: " + str(pos))
              pos2 = s.find(removeto, pos)
              #log("to pos: " + str(pos2))
              #log("pre removal: " + s)
              s = s[:pos] + s[pos2+len(removeto):]
              #log("post removal: " + s)

        if arraytype and s.count("=") == 1 and "{" in s and "}" in s:
          # We're defining an array on the fly, prepend the type
          s = s.replace("{", arraytype + basetype + "{", 1)

        # Use pointers to arrays for C arrays, also when declared with assignment
        if ("=" in s) and ("{" in s) and add_addr_of:
          s = s.replace("= [", "= &[")

        return s
    
    def visit_DeclList(self, n):
        s = self.visit(n.decls[0])
        if len(n.decls) > 1:
            s += ', ' + ', '.join(self.visit_Decl(decl, no_type=True) 
                                    for decl in n.decls[1:])
        return s
    
    def visit_Typedef(self, n):
        s = ''
        if n.storage: s += ' '.join(n.storage) + ' '
        s += self._generate_type(n.type)
        return s
    
    def visit_Cast(self, n):
        s = '(' + self._generate_type(n.to_type) + ')' 
        return s + ' ' + self.visit(n.expr)
    
    def visit_ExprList(self, n):
        visited_subexprs = []
        for expr in n.exprs:
            if isinstance(expr, c_ast.ExprList):
                visited_subexprs.append('{' + self.visit(expr) + '}')
            else:
                visited_subexprs.append(self.visit(expr))
        return ', '.join(visited_subexprs)
    
    def visit_Enum(self, n):
        s = 'enum'
        if n.name: s += ' ' + n.name
        if n.values:
            s += ' {'
            for i, enumerator in enumerate(n.values.enumerators):
                s += enumerator.name
                if enumerator.value: 
                    s += ' = ' + self.visit(enumerator.value)
                if i != len(n.values.enumerators) - 1: 
                    s += ', '
            s += '}'
        return s
        
    def visit_FuncDef(self, n):
        decl = self.visit(n.decl)
        self.indent_level = 0
        # The body is a Compound node
        body = self.visit(n.body)
        return decl + ' ' + body + '\n'

    def visit_FileAST(self, n):
        s = ''
        for ext in n.ext:
            if isinstance(ext, c_ast.FuncDef):
                s += self.visit(ext)
            else:
                s += self.visit(ext) + ';\n'
                # This could be just a function prototype
                for line in s.split("\n"):
                  if line.strip().startswith("func") and ("{" not in s):
                    # Delete this line
                    #log("Deleting: " + line)
                    s = s.replace(line, "")
                  #elif "func" in s:
                  #  log("Strange: " + s)
        return s

    def visit_Compound(self, n):
        s = self._make_indent() + '{\n'
        self.indent_level += 2
        if n.block_items:
            s += ''.join(self._generate_stmt(stmt) for stmt in n.block_items)
        self.indent_level -= 2
        s += self._make_indent() + '}\n'
        return s
    
    def visit_ParamList(self, n):
        return ', '.join(self.visit(param) for param in n.params)

    def visit_Return(self, n):
        s = 'return'
        if self.inmain:
          # Go can return in main, but not with a value
          return s
        if n.expr:
          s += ' ' + self.visit(n.expr)
        if ("==" in s) or (">" in s) or ("<" in s) or (">=" in s) or ("<=" in s) or ("!=" in s):
          #log("This function should really return a bool instead! " + self.current_function_name)
          if not self.current_function_name in self.should_return_bool_instead_of_int:
            self.should_return_bool_instead_of_int.append(self.current_function_name)
        return s

    def visit_Break(self, n):
        return 'break;'
        
    def visit_Continue(self, n):
        return 'continue;'
    
    def visit_TernaryOp(self, n):
        # Look at the type for the variable in n.conf if it's only one variable, find the type from the hashtable for types
        condition = self.visit(n.cond).strip()
        ctype = "bool"
        if condition in self.vartypes:
          ctype = self.vartypes[condition]
          #log(condition + " is of type " + ctype)
        if ctype == "bool":
          s = "map[bool]" + n.iftrue.type + "{true: " + self.visit(n.iftrue) + ", false: " + self.visit(n.iffalse) + "}[" + condition + "]"
        elif ctype == "int":
          s = "map[int]" + n.iftrue.type + "{1: " + self.visit(n.iftrue) + ", 0: " + self.visit(n.iffalse) + "}[" + condition + "]"
        return s
    
    def _remove_curly_blank_lines(self, s):
      """Remove curly brackets at beginning and at end, remove blank lines"""
      s = s.replace("{", "", 1)
      s = s.rsplit("}", 1)[0]
      lines = s.split("\n")
      bodylines = []
      for line in lines:
        if line.strip() != "":
          bodylines.append(line)
      s = "\n".join(bodylines) + "\n"
      return s
    
    def visit_If(self, n):
        s = 'if ('
        if n.cond:
          e = self.visit(n.cond)
          if e.isalnum():
              # TODO: Find out how to replace all variables that are evaluated
              #       on their own with > 0, since ints are so often booleans
              #
              # if it's just a lone variable, assume it is an int used as bool
              e = "" + e + " > 0" 
          s += e
        s += ') {\n'
        if_body_true = self._generate_stmt(n.iftrue, add_indent=True)
        if if_body_true.strip().startswith("{"):
          if_body_true = self._remove_curly_blank_lines(if_body_true)
        s += if_body_true
        if n.iffalse: 
            else_between = self._make_indent() + '} else {\n'
            if_body_false = self._generate_stmt(n.iffalse, add_indent=True)
            if if_body_false.strip().startswith("{"):
              if_body_false = self._remove_curly_blank_lines(if_body_false)
            # Only add the "else" part if it is not empty
            if if_body_false.strip() != "":
              s += else_between + if_body_false
        s += self._make_indent() + "}"
        
        return s
    
    def visit_For(self, n):
        s = 'for '
        if n.init:
          inits = self.visit(n.init)
          if "," in inits:
              # TODO: Fix this so that x=",", y="," as init can work...
              s = inits.replace(",", ";") + "\n"
              s += self._make_indent() + 'for '
          else:
              s += inits
        s += ';'
        if n.cond:
          e = self.visit(n.cond)
          if e.isalnum():
              # TODO: Find out how to replace all variables that are evaluated
              #       on their own with > 0, since ints are so often booleans
              #
              # if it's just a lone variable, assume it is an int used as bool
              e = e + " > 0" 
          else:
            # Just handle a few simple and common cases
            # TODO: Find a proper fix for integers in boolean expressions
            if ("&&" in e) or ("||" in e):
              if e.count("&&") == 1:
                if e.split("&&")[0].strip().isalpha():
                  word = e.split("&&")[0].strip()
                  e = e.replace(word, "(" + word + " > 0)", 1)
                elif e.split("&&")[1].strip().isalpha():
                  word = e.split("&&")[1].strip()
                  e = e.replace(word, "(" + word + " > 0)", 1)
              elif e.count("||") == 1:
                if e.split("||")[0].strip().isalpha():
                  word = e.split("&&")[0].strip()
                  e = e.replace(word, "(" + word + " > 0)", 1)
                elif e.split("||")[1].strip().isalpha():
                  word = e.split("&&")[1].strip()
                  e = e.replace(word, "(" + word + " > 0)", 1)
          s += e
        s += ';'
        multiple_nexts = False
        if n.next:
          nexts = ' ' + self.visit(n.next)
          if "," in nexts:
            s += ""
            multiple_nexts = True
          else:
            s += nexts
        genstmt = self._generate_stmt(n.stmt, add_indent=True)
        if not genstmt.strip().startswith("{"):
            s += " {\n"
        if genstmt.strip() != "":
            s += genstmt
        if multiple_nexts:
            # TODO: Fix this so that x++, y++ can work...
            nexts = nexts.replace(",", ";")
            s += "\n" + self._make_indent() + nexts.rstrip() + "\n"
        if s.count("}") < s.count("{"):
            s += self._make_indent() + "}"
        
        return s

    def visit_While(self, n):
        s = 'for ('
        if n.cond:
          pexp = ""
          cond = self.visit(n.cond)
          if "=" in cond:
            # If the assignment is surrounded by (), pick it out and put it
            # above the for-loop and befor the last } in the for loop.
            # Replace the () with the left side of the assignment.
            log("assignment in cond!")
            apos = cond.find("=")
            pos1 = cond.rfind("(", 0, apos)
            # find matching ) for the one at pos 1
            inside = 1
            found = False
            for i, c in enumerate(cond):
              #log("POS: " + str(i) + " INSIDE: " + str(inside))
              if i <= apos:
                continue
              if c == "(":
                inside += 1
              elif c == ")":
                inside -= 1
              if inside == 0:
                pos2 = i
                found = True
                break
            if found:
              pexp = cond[pos1+1:pos2]
              #log("PEXP: " + pexp)
              left = pexp.split("=")[0].strip()
              cond = cond.replace(pexp, left)
              s = pexp + "\n" + self._make_indent() + s
          s += cond
        s += ') '
        body = self._generate_stmt(n.stmt, add_indent=True)
        # TODO: Create a function of the curly-bracket fix
        if pexp:
          body += self._make_indent() + pexp
        if not body.strip().startswith("{"):
            s += " {\n"
        if body.strip() != "":
            s += body
        if s.count("}") < s.count("{"):
            s += self._make_indent() + "}"
        return s

    def visit_DoWhile(self, n):
        s = 'for'
        s += self._generate_stmt(n.stmt, add_indent=True)
        # Remove the last "}" in s
        s = "}".join(s.split("}")[:-1])
        if n.cond:
            con = self.visit(n.cond)
            if con != "1":
                s += self._make_indent() + 'if !('
                s += self.visit(n.cond)
                s += ') {break};\n'
        s += self._make_indent() + "}"
        return s

    def visit_Switch(self, n):
        s = 'switch (' + self.visit(n.cond) + ')\n'
        s += self._generate_stmt(n.stmt, add_indent=True)
        return s
    
    def visit_Case(self, n):
        s = 'case ' + self.visit(n.expr) + ':\n'
        s += self._generate_stmt(n.stmt, add_indent=True)
        return s
    
    def visit_Default(self, n):
        return 'default:\n' + self._generate_stmt(n.stmt, add_indent=True)

    def visit_Label(self, n):
        return n.name + ':\n' + self._generate_stmt(n.stmt)

    def visit_Goto(self, n):
        return 'goto ' + n.name + ';'

    def visit_EllipsisParam(self, n):
        return '...'

    def visit_Struct(self, n):
        return self._generate_struct_union(n, 'struct')

    def visit_Typename(self, n):
        return self._generate_type(n.type)
        
    def visit_Union(self, n):
        return self._generate_struct_union(n, 'union')

    def visit_NamedInitializer(self, n):
        s = ''
        for name in n.name:
            if isinstance(name, c_ast.ID):
                s += '.' + name.name
            elif isinstance(name, c_ast.Constant):
                s += '[' + name.value + ']'
        s += ' = ' + self.visit(n.expr)
        return s

    def _generate_struct_union(self, n, name):
        """ Generates code for structs and unions. name should be either 
            'struct' or union.
        """
        s = name + ' ' + (n.name or '')
        if n.decls:
            s += '\n'
            s += self._make_indent() 
            self.indent_level += 2
            s += '{\n'
            for decl in n.decls:
                s += self._generate_stmt(decl)
            self.indent_level -= 2
            s += self._make_indent() + '}'
        return s

    def _generate_stmt(self, n, add_indent=False):
        """ Generation from a statement node. This method exists as a wrapper
            for individual visit_* methods to handle different treatment of 
            some statements in this context.
        """
        typ = type(n)
        if add_indent: self.indent_level += 2
        indent = self._make_indent()
        if add_indent: self.indent_level -= 2
        
        if typ in ( 
                c_ast.Decl, c_ast.Assignment, c_ast.Cast, c_ast.UnaryOp,
                c_ast.BinaryOp, c_ast.TernaryOp, c_ast.FuncCall, c_ast.ArrayRef,
                c_ast.StructRef):
            # These can also appear in an expression context so no semicolon
            # is added to them automatically
            #
            return indent + self.visit(n) + '\n'
        elif typ in (c_ast.Compound,):
            # No extra indentation required before the opening brace of a 
            # compound - because it consists of multiple lines it has to 
            # compute its own indentation.
            #
            return self.visit(n)
        else:
            return indent + self.visit(n) + '\n'

    def _generate_decl(self, n):
        """ Generation from a Decl node.
        """
        s = ''
        if n.funcspec: s = ' '.join(n.funcspec) + ' '
        if n.storage: s += ' '.join(n.storage) + ' '
        s += self._generate_type(n.type)
        return s
    
    def _generate_type(self, n, modifiers=[]):
        """ Recursive generation from a type node. n is the type node. 
            modifiers collects the PtrDecl, ArrayDecl and FuncDecl modifiers 
            encountered on the way down to a TypeDecl, to allow proper
            generation from it.
        """
        typ = type(n)
        #~ print(n, modifiers)
        
        if typ == c_ast.TypeDecl:
            s = ''
            if n.quals: s += ' '.join(n.quals) + ' '
            s += self.visit(n.type)
            
            nstr = n.declname if n.declname else ''
            # Resolve modifiers.
            # Wrap in parens to distinguish pointer to array and pointer to
            # function syntax.
            #
            for i, modifier in enumerate(modifiers):
                if isinstance(modifier, c_ast.ArrayDecl):
                    if (i != 0 and isinstance(modifiers[i - 1], c_ast.PtrDecl)):
                        nstr = '(' + nstr + ')'
                    nstr += '[' + self.visit(modifier.dim) + ']'
                elif isinstance(modifier, c_ast.FuncDecl):
                    if (i != 0 and isinstance(modifiers[i - 1], c_ast.PtrDecl)):
                        nstr = '(' + nstr + ')'
                    nstr += '(' + self.visit(modifier.args) + ')'
                elif isinstance(modifier, c_ast.PtrDecl):
                    nstr = '*' + nstr
            if nstr: s += ' ' + nstr
            return s
        elif typ == c_ast.Decl:
            return self._generate_decl(n.type)
        elif typ == c_ast.Typename:
            return self._generate_type(n.type)
        elif typ == c_ast.IdentifierType:
            return ' '.join(n.names) + ' '
        elif typ in (c_ast.ArrayDecl, c_ast.PtrDecl, c_ast.FuncDecl):
            return self._generate_type(n.type, modifiers + [n])
        else:
            return self.visit(n)

    def _parenthesize_if(self, n, condition):
        """ Visits 'n' and returns its string representation, parenthesized
            if the condition function applied to the node returns True.
        """
        s = self.visit(n)
        if condition(n):
            return '(' + s + ')'
        else:
            return s

    def _parenthesize_unless_simple(self, n):
        """ Common use case for _parenthesize_if
        """
        return self._parenthesize_if(n, lambda d: not self._is_simple_node(d))

    def _is_simple_node(self, n):
        """ Returns True for nodes that are "simple" - i.e. nodes that always
            have higher precedence than operators.
        """
        return isinstance(n,(   c_ast.Constant, c_ast.ID, c_ast.ArrayRef, 
                                c_ast.StructRef, c_ast.FuncCall))

    def last_minute_replacements(self, s, firstname):
      # Insert the filename without extension as argv[0]
      if "argv[0]" in s:
        s = s.replace("argv[0]", '"' + firstname + '"')
      # Make replacements all over
      for r in WHOLE_PROGRAM_REPLACEMENTS:
        if r in s:
          newstring = WHOLE_PROGRAM_REPLACEMENTS[r]
          # Special cases
          if "flag.Args" in newstring:
            if "flag" not in self.imports:
              self.imports.append("flag")
          s = s.replace(r, newstring)
      # Fix ++ and --
      while s.count("$$$") > 0:
        pos1 = s.find("$$$") + 3
        pos2 = s.find("$$$", pos1)
        contents = s[pos1:pos2]
        fixed_cont = contents.replace("++", "").replace("--", "")
        s = s.replace("$$$" + contents + "$$$", fixed_cont)

        line = s[s.rfind("\n", 0, pos1)+1:s.find("\n", pos1)]
        whitespace = line[:len(line) - len(line.lstrip())]

        # Ok, we have removed ++ or -- from the line, let's add it again on the line above or below
        contents = contents.strip()
        if contents.startswith("++") or contents.startswith("--"):
          # insert pos should be the line above
          inspos = s.rfind("\n", 0, pos1) + 1
          if contents.startswith("++"):
            contents = contents.replace("++", "") + "++"
          elif contents.startswith("--"):
            contents = contents.replace("--", "") + "--"
          s = s[:inspos] + whitespace + contents + "\n" + s[inspos:]
        elif contents.endswith("++") or contents.endswith("--"):
          # insert pos should be the line below
          inspos = s.find("\n", pos1) + 1
          s = s[:inspos] + whitespace + contents + "\n" + s[inspos:]
        #break
      for fix in range(s.count("]byte = new([")):
        pos = s.find("]byte = new([")
        bpos = s.rfind("\n", 0, pos)
        eolpos = s.find("\n", pos)
        contents = s[bpos:eolpos]
        log("AJAJAJAJ! " + contents)
        num = "".join([x for x in contents.split("=")[1] if x in "0123456789"])
        log("NUM: " + num)
        if num:
          newcontents = contents.split("*")[0] + "CString = NewEmptyCString(" + num + ")"
        else:
          newcontents = contents + "CString"
        s = s[:bpos] + newcontents + s[eolpos:]
      # Use fmt.Println instead of fmt.Printf ... \n
      for fix in range(s.count("fmt.Printf(")):
        pos = s.find("fmt.Printf(")
        eolpos = s.find("\n", pos)
        contents = s[pos:eolpos]
        if contents.strip().endswith("\\n\")") and contents.count("\"") == 2:
          #log("FOUND: " + contents)
          newcontents = contents.replace("fmt.Printf(", "fmt.Println(", 1).replace("\\n\")", "\")", 1)
          #log("NEWCONTENTS: " + newcontents)
          s = s[:pos] + newcontents + s[eolpos:]
      #for sfunc in ["Printf", "Scanf", "Println", "Scanln"]:
      #  if (sfunc + "(NewCString") in s:
      #      s = s.replace(sfunc + "(NewCString", sfunc + "(")
      #      s = s.replace("strlen(", "len(")
      return s

    def fix_int_to_bool(self, s):
      #log("Fix int to bool")
      for fn in self.should_return_bool_instead_of_int:
        pos = s.find("func " + fn)
        #log("Found " + fn + " at " + str(pos))
        eolpos = s.find("\n", pos)
        #log("Found eol at " + str(eolpos))
        intpos = s.rfind(" int ", pos, eolpos)
        if intpos < eolpos:
          #log("This is an int function, yes! " + str(pos) + " " + str(intpos) + " " + str(eolpos))
          #log(s[intpos:eolpos])
          s = s[:intpos] + "bool" + s[intpos+4:]
      return s

def cleanup(data):
  lines = []
  for line in data.split("\n"):
    if "#include" in line:
      continue
    if line.strip().startswith("//"):
      continue
    lines.append(line)
  return "\n".join(lines)

def translate_to_go(filename):

    firstname = filename 
    if "." in filename:
      firstname = filename.rsplit(".", 1)[0]

    clearlog()

    f = open(filename)
    data = f.read()
    f.close()

    data = cleanup(data)
   
    filename = tempfile.mkstemp()[1]
    f = open(filename, "w")
    f.write(data)
    f.close()

    ast = parse_file(filename, use_cpp=True)
    generator = GoGenerator()
    s = generator.visit(ast)
    s = generator.fix_int_to_bool(s)
    s = generator.last_minute_replacements(s, firstname)
    s = generator.make_header() + s

    print(s)

def log(s):
  f = open("/tmp/c2go.log", "a")
  f.write(str(s) + "\n")
  f.close()

def clearlog():
  f = open("/tmp/c2go.log", "w")
  f.close()

#------------------------------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) > 1:
        translate_to_go(sys.argv[1])
    else:
        print("Please provide a filename as argument")

