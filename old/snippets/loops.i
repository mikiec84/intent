# An anonymous, infinite Loop.
Loop:
    # "pass" is analogous to the "pass" keyword in python; essentially it
    # is a no-op or placeholder.
    pass

# An infinite Loop named "forever".
Loop forever:
    Process events()

# An infinite Loop named "as long as it takes".
Loop as long as it takes:
    If (Stop was requested()):
        Break
    Keep working()

# Loop over a numeric range that starts with zero, visiting every value once.
# We could do a classic 3-part specIfication of the Loop, but the short form
# just declares the upper bound. Note the use of the IN operator, -[.
Loop all rows in table (row -[ row count):
    Loop all fields in row (field -[ field count):
        If (fields[field].value == 42):
            # This demonstrates the "named Break" feature. We Break out of
            # both Loops with this statement, not just out of the inner one.
            Break all rows

# Loop over a numeric range, visiting every value once.
Loop each month of the year (month -[ +<range[1..12]):
    Make calendar(month)

# Loop over a numeric range, visiting every value once, alternate form.
Loop every month (month:= 1; month <= 12; ++month):
    Report record high temperature(month)

# Loop over a numeric range, visiting odd months only, and skipping month 7.
# Loops that do not have an informal name are identIfied by their iteration
# variable, so this Loop can be called "Loop month".
Loop (month: 1; month <= 12; month += 2):
    If (month == 7):
        # "continue" has the same meaning as in C, java, and python; begin next
        # iteration of the Loop.
        continue
    Do something(month)

# Invalid; can't use +<backwards on Loops with 3-part specIfier.
Loop +<backward (month: 1; month <= 12; month += 2):
    Do something(month)

# Loop over a numeric range, visiting every value once -- backwards.
Loop each month of the year (month -[ +<range[12..1]):
    Print month name(month)

# Loop over a numeric range, visiting every value once -- backwards, alternate
# form.
Loop +<backward; each month of the year (month -[ +<range[12..1]):
    Generate spending report(month)

# Loop over a numeric range, visiting every value once -- backwards, alternate
# form.
Loop (month:= 12; month >= 1; --month):
    Print(Get month name(month))

# Loop over a container, visiting every value once.
Loop (school -[ high schools):
    Print(school.mascot)

# Loop over a container, visiting every value once, in reverse order
Loop +<backward (school: high schools):
    school.Find valedictorian()

# Loop over an enum, visiting every value once.
Loop +<backward (day -[ weekdays):
    Print(To string(day))

# Nested Loops.
Loop (year -[ recent years):
    Loop (month: +<range[1..12]):
        day count := Get day count(month, year)
        If (day count == 29):
            # A "named continue" can begin the next iteration of any containing
            # Loop, not just the innermost one# Here, we skip from Feb 1980
            # to Jan 1981; I guess nothing interesting ever happens after the
            # first 30 days of a leap year. :-)
            continue year
        for (day: day count):
            Show interesting trivia for day(year, month, day)

# Analog to do...while in C.
Loop:
    If (--n < 1 || Is space(my str[n])):
        Break

