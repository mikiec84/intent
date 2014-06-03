node type: table
    columns:
        - name
        - link to class
    rows:
        compiler:
            for c in syntax.classes where c :: code element:
                print "{c.name}|@{c.url}"
