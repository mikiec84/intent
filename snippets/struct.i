# This is a schema for a struct. The things that are baked into the language
# at its core are super primitive:
# * the keyword "schema"
# * the syntax for naming, and the +name decorator
# * the syntax for defining
struct: schema
    name
    properties: statements//namelines//names



marks = mark*

name (alias): datatype marks = value