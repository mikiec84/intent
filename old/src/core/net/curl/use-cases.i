browser-style parallel downloads: use case
    summary---Need to start multiple downloads, each of which might take a long
    ... time to finish. Downloads each have a separate progress, and need to be
    ... individually cancellable.
    participants:
        - user---requested the downloads; may initiate cancel
        - progress thread---iterates over pending downloads, fetching current
        ... status and updating a UI.
        - ui---exposes a cancel button on each pending download, displays
        ... progress
        - ui handler thread---runs the cancel button handler
    requirements:
        - scale---Although UIs may choose to optimize for only a handful of
        ... parallel downloads, it needs to be possible to manage hundreds of
        ... thousands of parallel downloads efficiently. Machines that have
        ... fat network pipes and muliple NICs, and that are doing unattended
        ... work at scale, may need that scale.
