# An anonymous, infinite loop.
loop:
    # "pass" is analogous to the "pass" keyword in python; essentially it
    ... is a no-op or placeholder.
    pass

# An infinite loop named "forever".
loop forever:
    Process events()

# An infinite loop named "as long as it takes".
loop as long as it takes:
    if (Stop was requested()):
        break
    Keep working()

# Loop over a numeric range that starts with zero, visiting every value once.
# We could do a classic 3-part specification of the loop, but the short form
# just declares the upper bound. Note the use of the IN operator, -[.
loop all rows in table (row -[ row count):
    loop all fields in row (field -[ field count):
        if (fields[field].value == 42):
            # This demonstrates the "named break" feature. We break out of
            ... both loops with this statement, not just out of the inner one.
            break all rows

# Loop over a numeric range, visiting every value once.
loop each month of the year (month -[ +<range[1..12]):
    Make calendar(month)

# Loop over a numeric range, visiting every value once, alternate form.
loop every month (month:= 1; month <= 12; ++month):
    Report record high temperature(month)

# Loop over a numeric range, visiting odd months only, and skipping month 7.
... Loops that do not have an informal name are identified by their iteration
... variable, so this loop can be called "loop month".
loop (month: 1; month <= 12; month += 2):
    if (month == 7):
        # "continue" has the same meaning as in C, java, and python; begin next
        ... iteration of the loop.
        continue
    Do something(month)

# Invalid; can't use +<backwards on loops with 3-part specifier.
loop +<backward (month: 1; month <= 12; month += 2):
    Do something(month)

# Loop over a numeric range, visiting every value once -- backwards.
loop each month of the year (month -[ +<range[12..1]):
    Print month name(month)

# Loop over a numeric range, visiting every value once -- backwards, alternate
... form.
loop +<backward; each month of the year (month -[ +<range[12..1]):
    Generate spending report(month)

# Loop over a numeric range, visiting every value once -- backwards, alternate
... form.
loop (month:= 12; month >= 1; --month):
    Print(Get month name(month))

# Loop over a container, visiting every value once.
loop (school -[ high schools):
    Print(school.mascot)

# Loop over a container, visiting every value once, in reverse order
loop +<backward (school: high schools):
    school.Find valedictorian()

# Loop over an enum, visiting every value once.
loop +<backward (day -[ weekdays):
    Print(To string(day))

# Nested loops.
loop (year -[ recent years):
    loop (month: +<range[1..12]):
        day count := Get day count(month, year)
        if (day count == 29):
            # A "named continue" can begin the next iteration of any containing
            ... loop, not just the innermost one... Here, we skip from Feb 1980
            ... to Jan 1981; I guess nothing interesting ever happens after the
            ... first 30 days of a leap year. :-)
            continue year
        for (day: day count):
            Show interesting trivia for day(year, month, day)

# Analog to do...while in C.
loop:
    if (--n < 1 || Is space(my str[n])):
        break

