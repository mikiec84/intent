## Specifies that a string contains short, single-line identifiers.
name: mark
    properties:
        - valid size: range of int = [1..128]

    adjust marks: -<multiline

    Make(valid size): +<copyargs +<inheritargs ctor

    Can bind(): mark.Can bind
        answer = type(site) is str
