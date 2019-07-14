: design by contract.*

flags: enum # enum is a table where first column is name and second is number; more columns can be added
    name | number: bitmask | abbrev: str
    size ok
    deserialize ok
    keys ok
    values ok
    confidentiality
    integrity
    authenticated origin
    nonrepudiation
    perfect forward secrecy (pfs)
    uniqueness
    limited scope

compiler: # compiler is partial class that can be extended anywhere
    Assign abbrevs:
        params:
            table: enum
            text column: +index
            abbrev column: +index
        Loop row -[ table:
            txt := row[text column]
            Loop i -[ range[1..Len(txt)]:
                prefix := txt[:i]
                collides := false
                Loop other -[ table:
                    If other.number == row.number:
                        Continue other
                    If row[text column].Starts with(prefix):
                        collides := true
                        Break other
                If !collides:
                    row[abbrev column] = prefix

compiler.Assign abbrevs(flags, 0, 2)

message trust context (mtc): class
    "Describe the trust guarantees associated with a given message.
    ... See http://bit.ly/2UutabT for more information."

    properties:
        affirmed: bitmask = 0
        denied: ditto

    Make: mtc +copyargs
        params:
            affirmed: +fromprop +optional
            denied: ditto
        Affirmed and denied can't overlap: precondition
            affirmed & denied == 0
        # No body is provided, but copyargs assures that
        # assignment happens from args to properties.
        # The precondition is also called automatically if
        # it isn't called explicitly.

    # Methods have the following special keywords defined:
    #   this = object being called
    #   this.class = class of this
    #   this.prototype = object this one came from?
    #   thismethod = the function being called
    #   thismethod.implementing class

    Affirm: #implicitly, methods have "this", and "thismethod" names defined, and are +const
        params:
            flags: bitmask
        affirmed |= flags
        denied &= ~flags

    Deny:
        params:
            flags: bitmask
        denied |= flags
        affirmed &= ~flags

    Undefine:
        params:
            flags: bitmask
        affirmed &= ~flags
        denied &= ~flags

    Trust for: nullable<bool>
        "Tells what trust applies for the given flag -- True if trusted, False if explicitly
        ... not trusted, or None if trust for that flag has not been evaluated."
        params:
            flags: bitmask
        If (affirmed & flag) == flag: Return true
        If (denied & flag) == flag: Return false
        Return null

    Get flag for label: bitmask -this
        params:
            label: str
        Loop row -[ flags:
            If row.name.Starts with(label):
                Return row.number
        Return 0

    Make from text: mtc
        params:
            txt: str
        x := mtc.Make()
        If txt:
            If txt != '?':
                # Scenarios: A) start with something like +a+i-n-p; B) start with -n-p+a+i
                pluses := txt.Replace(' ', '').Lower().Split('+')
                # Now we'll have: A) ['','a','i-n-p']; B) ['-n-p','a','i']
                Loop plus -[ pluses:
                    If !plus:
                        Continue
                    minuses := plus.Split('-')
                    If minuses[0]:
                        # This is really a plus followed by minuses
                        x.Affirm(Get flag for label(minuses[0]))
                    Loop minus -[ minuses[1:]:
                        If minus:
                            x.Deny(Get flag for label(minus))
        Return x

    .Get text: -this
        params:
            flags: bitmask
            labels: list<str>
            mark: char
            spacer: str
        x := ''
        i := 0
        n := 1
        Loop n < _MAX_FLAG:
            If (flags & n) == n:
                x = x + spacer + mark + labels[i]
            n *= 2
            i += 1
        Return x.lstrip()

    Get abbrevs: +property
        "Return an abbreviated string that summarizes which flags are set and explicitly unset."
        pluses := .Get text(affirmed, flags.abbrev, '+', '')
        minuses := .Get text(affirmed, flags.abbrev, '-', '')
        Return (!pluses && !minuses) ? '?' : pluses + minuses

    Get labels: +property
        "Return a long-form string that describes which flags are set and explicitly unset."
        pluses := .Get text(affirmed, flags.name, '+', ' ')
        minuses := .Get text(affirmed, flags.name, '-', ' ')
        If pluses && minuses: Return pluses + ' ' + minuses
        If pluses: Return pluses
        If minuses: Return minuses
        Return '?'

    To str:
        Return abbrevs