# Hyperlinks, Anchors, and Interjections

Intent is a powerful hypertext format. It supports all of the linking constructs that HTML has popularized, as well as footnote, endnote, and indexing features offered by word processors and desktop publishing software. This provides what linguists call __deixis__ -- the ability to point with our language.

## Anchors

An anchor is a target for a hyperlink -- something we can point at. It may be a word or phrase, a location, a visual artifact, or similar.

All formally assigned identifiers (block headers that end in a colon and are followed by indented content) in intent are automatic anchors at their place of definition. In this code:

```i
Register to vote: bool
    params:
        voter
        election
    code:
        # code goes here
```

...the name `Register to vote` is an anchor. So is `Register to vote.params` and `Register to vote.code`.

In addition, formal anchors can be created by enclosing a word or phrase in square brackets. For example, this snippet creates anchors around two terms that are being defined, so the terms can be referenced elsewhere:

```i
[Magnetic resonance imaging] ([MRI]) is a medical imaging technique used
in radiology to form pictures of the anatomy and the physiological
processes of the body.
```

Anchors are not case sensitive; references to these anchors that use `MAGNETIC RESONANCE IMAGING` or `mri` as the anchor's identifiers will point to the same things. When text is anchored in this way, sometimes inflections (plurals, conjugations) of the text should also 

Sometimes the formal identifier associated with an anchor needs to be different from the words it brackets. Use the more formal syntax `[id: informal text]` in such cases:

```i
The [ww2history: history of World War II] is long and complex.
```   

To anchor a location instead of a name, simply insert a formally identified anchor at the desired place without any informal text:

```i
running text [anchorpoint:] more running text
```

When anchors encompass content that is large or complex, a __paired variant__ is used. This is somewhat like a "begin tag" and an "end tag" in HTML. The first half of a pair variant contains the formal identifier followed by whitespace and an elipsis, ` ...`. The second half is another bracketed expression that contains the same identifier preceded by slash:

```i
The Gettysburg Address is a famous speech given by Abraham Lincoln.
It goes like this:

    [gaddr-text ...]
    (all the text of the speech)
    [/gaddr-text]
```

...or, in an acceptable shorthand, brackets around a slash followed by any subset of the identifier in the first half, including the empty string:

```i
    [gaddr-text ...]
    (all the text of the speech)
    [/]
```

## Hyperlinks

A hyperlink is an expression in the form `[id@anchor: clickable content]`, where `id` is optional and has the same semantics as with anchors (allowing a hyperlink to also be an anchor itself), `anchor` is a target bracketed elsewhere, and `clickable content` is the text or graphic that would be rendered as blue underlined text if the hyperlink were HTML. Additional params--`target` and `base`--can be added between `anchor` and `linked text` using `;` + whitespace, as in:

```i
[@https://www.example.com/a/b; target=_blank; rel=author: clickable text]
```

Hyperlinks to simple textual anchors can be shortened so they look like those anchors with an `@` in front of them. To refer to the MRI term from our example above, running text might contain the following hyperlink:

```i
You need to get an [@MRI] as soon as possible.
```  

Hyperlinks also support a paired variant that follows the same rules as anchors:

```i
[sample@https://a.b.com/c; target=_blank ...]clickable content[/] 
```

...or:

```i
[sample@https://a.b.com/c; target=_blank ...]clickcable content[/sample]
```

## Interjections

An interjection is a way of managing linked content where it is convenient to create and maintain it, but displaying it somewhere else. Interjections are intent's way of dealing with things like footnotes, endnotes, callouts, and similar collateral. In an editor, iterjections are defined inline: inside, next to, or near the content they enhance, in an editor. However, they may be rendered in an entirely different place, such as a footer, an appendix, etc.

An interjection may have one or more __display points__ where its content or some subset or transformation thereof is rendered. 

A display point for interjection is defined with an expression in the form `[^@id: anchor content]`, where `id` is a formal identifier for the interjection.

The interjected content (e.g., the text of a footnote, the diagram) is defined at a __definition point__. This may be an anchor in the form `[^id: interjected content]`, or a formally named block that has a type, marks, and other properties. In the latter case, the block should carry the `+interject` mark to indicate that it is not to be displayed as part of the running context.

Suppose you are using intent to write about some product requirements, and you want to add a footnote about . You might do it like this:

```i
We need to make sure that we consider accessibility. Some of our
customers [^@1: say] that they won't buy unless the product is
usable by the visually impaired.

[^1: See the focus group done in Sep 2019.]
```

You could also do it like this:

```i
We need to make sure that we consider accessibility. Some of our
customers [^@1: say] that they won't buy unless the product is
usable by the visually impaired.

^1: footnote
    See the focus group done in Sep 2019.
```




