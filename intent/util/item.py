import colors
import os
import sys

_colors_supported = 'TERM' in os.environ and os.environ['TERM'] != 'unknown'
using_colors = _colors_supported
_uncolored = lambda x: x
_error_colorizer = colors.red
_warning_colorizer = colors.yellow


def _indent(txt, indent_count=1):
    return '    ' * indent_count + txt


def _wrap_indented(txt, indent_count=1):
    return txt.replace('\n', '\n' + '    ' * indent_count)


def pluralize(noun, n):
    return noun if n == 1 else noun + 's'


def summarize(warning_count, error_count):
    msg = str(warning_count) + ' ' + pluralize('warning', warning_count)
    msg += ', ' + str(error_count) + ' ' + pluralize('error', error_count)
    return msg


def _write(msg):
    sys.stdout.write(msg)
    

class PrettyProcessedItem:
    def __init__(self, name, indent=0):
        self.name = name
        self.indent = indent
        self.details_exist = False
        self.error_count = 0
        self.warning_count = 0
        colorizer = _uncolored if not using_colors else colors.white
        _write(colorizer(name))

    def _add_details(self):
        if not self.details_exist:
            _write(':\n')
            self.details_exist = True

    def _report(self, code, label, msg):
        self._add_details()
        colorizer = _uncolored if not using_colors else (_error_colorizer if label == 'Error' else _warning_colorizer)
        _write(_indent(colorizer(label + ' ' + code), self.indent + 1) + ' -- ')
        msg = msg.strip()
        if '\n' in msg.strip():
            _write(_wrap_indented('\n' + msg, self.indent + 2) + '\n')
        else:
            _write(msg + '\n')

    def report_warning(self, code, msg):
        self.warning_count += 1
        self._report(code, 'Warning', msg)

    def report_error(self, code, msg):
        self.error_count += 1
        self._report(code, 'Error', msg)

    def finish(self, output=None):
        if not self.details_exist:
            _write('\r')
            colorizer = _uncolored if not using_colors else colors.green
            _write(_indent(colorizer(self.name), self.indent))
        if output:
            output = output.strip()
        if output:
            _write(_indent(output, self.indent + 1 if self.details_exist else 0) + '\n')
        elif not self.details_exist:
            _write('\n')


class MarkdownProcessedItem:
    def __init__(self, name, indent=0):
        self.name = name
        self.indent = indent
        self.details_exist = False
        self.error_count = 0
        self.warning_count = 0
        _write(_indent('- ' + name, self.indent))

    def _add_details(self):
        if not self.details_exist:
            _write(':\n')
            self.details_exist = True

    def _report(self, code, prefix, msg):
        self._add_details()
        _write(_indent('- ' + prefix + ' ' + code + ' -- ', self.indent + 1))
        msg = msg.strip()
        if '\n' in msg.strip():
            _write(_wrap_indented('```\n' + msg + '\n```', self.indent + 2) + '\n')
        else:
            _write(msg + '\n')

    def report_warning(self, code, msg):
        self.warning_count += 1
        self._report(code, 'Warning', msg)

    def report_error(self, code, msg):
        self.error_count += 1
        self._report(code, 'Error', msg)

    def finish(self, output=None):
        if output:
            output = output.strip()
        if output:
            _write(_indent(output, self.indent + 1 if self.details_exist else 0) + '\n')
        elif not self.details_exist:
            _write('\n')
