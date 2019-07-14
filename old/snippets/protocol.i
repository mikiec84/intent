Buy a house: protocol
  roles:
    - buyer:
      state machine:
        @startuml
        [*] -> State1
        State1 --> State2 : Succeeded
        State1 --> [*] : Aborted
        State2 --> State3 : Succeeded
        State2 --> [*] : Aborted
        state State3 {
          state "Accumulate Enough Data\nLong State Name" as long1
          long1 : Just a test
          [*] --> long1
          long1 --> long1 : New Data
          long1 --> ProcessData : Enough Data
        }
        State3 --> State3 : Failed
        State3 --> [*] : Succeeded / Save Result
        State3 --> [*] : Aborted
        @enduml
    - seller
    - buyer realtor
    - seller realtor
    - home inspector
    - title insurer
    - underwriter
    - mortgage broker
  nouns:
    - house
    - home inspection report
