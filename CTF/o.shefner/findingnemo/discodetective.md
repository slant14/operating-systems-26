# CTF `discodetective` — report

**Instance:** `findingnemo` (`threadctf`).
**Challenge:** `discodetective`.

## Locating the game

The container's `/home/ctf` directory contains no challenge files. The `python3 /root/discodetective.py` process listens on TCP port `9999` at `127.0.0.1`. These commands helped locate it:

```bash
ps -ef
cat /proc/net/tcp
nc 127.0.0.1 9999
```

Port `270F` in `/proc/net/tcp` is `9999` in decimal. We used the local [`discodetective_client.py`](discodetective_client.py) to complete the challenge; it automatically responded to time-limited `!!THOUGHT-STRIKE` prompts:

```bash
python3 /home/ctf/discodetective_client.py
```

The suspects, skills, choices, and facts change with each new connection. Therefore, the choice and fact numbers below apply **only to the recorded successful run**.

## Investigation steps

Skills: `LOGIC=2 EMPATHY=6 DRAMA=4 VOLITION=4 INLAND_EMPIRE=6 PHYSICAL=6 RHETORIC=4 SUGGESTION=4`. Route: `war-room → pipeline → founder → channel → kitchen → review`.

| Room | Choice | Result |
|---|---|---|
| `war-room` | `2` — find the deployment preceding the negative balance | `f1`: deployment at **03:05** (`deploy-time`) |
| `pipeline` | `2` — ask HELM-9 who requested the deployment | `f2`: the request came from the incident channel (`deploy-path=channel`) |
| `founder` | `1` — read the notes by the monitor | An additional question was unlocked in `kitchen` (`!!UNLOCK scene=kitchen`) |
| `channel` | `4` — ask who can trigger a deployment from the channel | `f3`: the channel administrators are Teodor Lindqvist and Nikolai Brandt (`channel-admins`) |
| `kitchen` | `4` — ask what people were whispering about at the card table | `f4`: exit log (`badge-log`); `f5`: badge owners (`badge-map`) |

In log `f4`, badge `9303` was used to exit at **02:17**, and `5488` at **03:17**. Map `f5` associates `9303` with Nikolai Brandt and `5488` with Teodor Lindqvist.

## Conclusion and proof

According to `f2` and `f3`, only the incident channel administrators, Teodor or Nikolai, could have triggered the deployment. The deployment occurred at **03:05** (`f1`). Nikolai left at **02:17**, before the deployment (`f4`, `f5`), so he could not have triggered it. Teodor left later, at **03:17**. During review, the game displayed `closable=1`.

Commands for the recorded run:

```text
ACCUSE 4
PROOF f1 f2 f3 f4 f5
```

Number `4` corresponded to Teodor Lindqvist. The saved investigation identified the culprit.
