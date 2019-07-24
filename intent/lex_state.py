NEW_LINE = TERM = NAME = HYPERTEXT = DESCRIPTOR = LEX_ERROR = PARAGRAPH = 0


def load():
    x = ['']
    g = globals()
    i = 1
    for name in g:
        if isinstance(g[name], int):
            g[name] = i
            x.append(name)
            i += 1
    del g['load']
    return x


names_by_index = load()


class LexState:
    def __init__(self, text: str):
        self._stack = []
        self.text = text
        self.i = 0
        self.indent = 0
        self.line_num = 0
        self.line_start = 0

    @property
    def state(self):
        return self._stack[-1] if self._stack else 0

    @property
    def end(self):
        return len(self.text)

    @property
    def col(self):
        return self.i - self.line_start

    def push(self, state: int):
        self._stack.append(state)

    def pop_until(self, until):
        while self._stack and (self.state != until):
            self._stack.pop()

    def pop(self):
        return self._stack.pop()

    def __len__(self):
        return len(self._stack)

    def __str__(self):
        stack = 'X->' + '->'.join([names_by_index[i] for i in self._stack])
        next = self.text[self.i:self.i+1]
        if not next:
            next = "(EOF)"
        return f"line {self.line_num}, col {self.col}, offset {self.i}, indent {self.indent} ({repr(next)}): " + stack
