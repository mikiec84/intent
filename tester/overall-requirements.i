parameter prediction: requirement
    given:
        A function or method that compiles
    when:
        A signal is received that analysis should be performed
    then:
        for each parameter:
            Determine some useful test values for it.
            Execute the function with the test values.
            Record the results.
            Ask the user if the output matches their expectations.
            if yes, record the inputs and outputs as new unit tests.
            
