
vehicle: class
    properties:
        - make: +< name
        - model: +< name
        - year: int +< range[1890..)
        - commands: list//str
    Make: ctor +< copyargs
