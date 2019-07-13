# Code as hypertext

Source code has always shared some properties with hypertext. When a compiler emits an error, and it identifies the file, line and offset where the error was detected, it is offering a sort of hyperlink that the programmer can follow to visit that site in code. Sometimes the textual representation of the location is even clickable. Modern IDEs treat function and class declarations as anchors, and generate a sort of browsable "site map" of the code. Collapsible comments or code regions are a form of StretchText--another idea under the hypertext umbrella.

However, source code is still not as rich as the forms of hypertext and hypermedia that the web has taught us to love:

Traditionally, source code is all textual.
GUI tools may allow you to attach to icons or palettes, but try to do the same thing in the middle of a function, and you'll find the experience suboptimal. And when was the last time your editor shows an inline swim lanes diagram or UML, so that you edited the two together and could triangulate from both visual and textual descriptions? How come source code is not inherently capable of linking to bugs or design docs? Why can't we fail a build if a we detect a project link to a project plan or a test plan or a golden thread or a formally defined persona?
Many constructs in code cannot be the targets of a link.
If you've ever used a static analysis tool like lint or Coverity or CLang,