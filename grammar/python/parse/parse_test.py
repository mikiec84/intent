import os, sys, unittest

from parse import lexer

def tokenize(txt):
  lex = lexer.Lexer()
  tokens = [tuple for tuple in lex(txt)]
  return tokens

class LexerTest(unittest.TestCase):

  def test_unique_tokens(self):
    tokens = [t for t in dir(lexer) if t.startswith('t_')]
