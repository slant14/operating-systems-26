The game is a sequence of rooms. Every intrusive thought opens a 3000 ms WARD window, and the only valid answer is WARD `sigil` `verb` `formed-urge`: the verb comes from the thought flavor (COMPULSION -> FORBID, MORTIFICATION -> DISMISS, ...), and the urge is the action in kebab, reversed, or shout form. Each successful room check gives one fact. change + staffing narrow the suspects to one workstream, and deploy-time + badge-log + badge-map remove everyone whose badge was out before the deploy. The review room then expects ACCUSE `n` followed by PROOF with exactly the facts that leave one suspect.

```python

if line.startswith("!!WARD-WINDOW"):
    for sigil, t in pending.items():
        send(f"WARD {sigil} {t['verb']} {formed(t['action'], t['form'])}")

send("ACCUSE 2")
send("PROOF f1 f2 f3 f4 f5")

```