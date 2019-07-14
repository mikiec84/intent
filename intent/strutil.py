from .dbc import precondition

def truncate_if_needed(text, max_len):
    if not (text is None):
        if len(text) <= max_len:
            return text
        else:
            return text[:max_len - 3] + '...'