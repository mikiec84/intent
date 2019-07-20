from typing import Union

from .strutil import truncate_if_needed
from .tok_types import *


class Token:
    def __init__(self, ls, ttype: Union[str, int], begin: int, end: int = None):
        self._ttype = ttype
        self._text = ls.text
        self._begin = begin
        self._line_offset = begin - ls.line_start
        if end is None:
            end = begin + 1
        self._end = end

    @property
    def line_num(self):
        return self.ls.line_num

    @property
    def line_offset(self):
        return self._line_offset

    @property
    def ttype(self):
        return self._ttype

    @property
    def begin(self):
        return self._begin

    @property
    def end(self):
        return self._end

    @property
    def value(self):
        return self._text[self.begin:self.end]

    def __len__(self):
        return self.end - self.begin

    def __str__(self) -> str:
        s = names_by_value[self.ttype]
        if self.ttype in [NAME, TEXT]:
            s += ': "' + truncate_if_needed(self.value, 10) + '"'
        s += f" at [{self.begin}:{self.end}]"
        return s

