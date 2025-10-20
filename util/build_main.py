import sys
import os
import threading
import subprocess
from time import sleep

#                       "PRE-"      "ALPH"      "A "
PRE_ALPHA: list[int] = [0x7072652D, 0x616C7068, 0x6120]
EMPTY: list[int] = 0x20202020

def commit_hash_main(stop_event: threading.Event):
    """
    Monitors the build ISO, and writes our current git commit hash into the
    version string if it is empty.
    """

    write_now = False
    write_pos = -1

    while not stop_event.is_set():
        if not write_now:
            sleep(1)
            try:
                with open("../../../build/ctr-u_CTR-Randomizer.bin", "rb") as in_file:
                    in_file.seek(0xE000)
                    for _ in range(0x2000):
                        chunk = int.from_bytes(in_file.read(4), "big")
                        if chunk == '':
                            break
                        if chunk == PRE_ALPHA[0]:
                            if int.from_bytes(in_file.read(4), "big") == PRE_ALPHA[1]:
                                if int.from_bytes(in_file.read(2), "big") == PRE_ALPHA[2]:
                                    write_pos = in_file.tell()
                                    if int.from_bytes(in_file.read(4), "big") == EMPTY:
                                        write_now = True
                                else:
                                    int.from_bytes(in_file.read(2), "big")
            except FileNotFoundError:
                sleep(1)
        else:
            # Find git commit hash
            subprocess_result = subprocess.run(
                ["git", "log", "--oneline", "--abbrev=8", "-n", "1"],
                capture_output=True
            )
            git_commit: str = subprocess_result.stdout.decode(encoding="utf-8")
            git_commit_hash: str = git_commit[:8]

            # Check if workspace clean or not. If not, alter hash slightly
            subprocess_result = subprocess.run(
                ["git", "status", "--porcelain"],
                capture_output=True
            )
            open_changes: str = subprocess_result.stdout.decode(encoding="utf-8")
            if len(open_changes) > 0:
                git_commit_hash = f"{git_commit_hash}+"

            # Write hash to ISO
            with open("../../../build/ctr-u_CTR-Randomizer.bin", "r+b") as in_file:
                in_file.seek(write_pos)
                in_file.write(str.encode(git_commit_hash, encoding="utf-8"))
            write_now = False


def build_main():
    """
    Instead of just running the main.py file of the psx-modding-toolchain, we
    import it here to run it, and surround it with a thread that monitors
    the build-ISO. If the build-ISO exists with our version string inside of it,
    but without a git commit hash attached, then we have the thread detect
    the current commit hash and write it into the ISO.
    """

    # prepare import path so we can actually import the main of the modding toolchain
    NEW_SCRIPT_DIR = os.path.dirname(os.path.realpath(f"{__file__}/../../../../../../../tools/mod-builder/main.py"))
    sys.path.append(os.path.dirname(f"{NEW_SCRIPT_DIR}\\"))

    # launch our script that writes commit hash into ISO building process
    stop_event = threading.Event()
    hash_thread = threading.Thread(target=commit_hash_main, args=[stop_event])
    hash_thread.start()

    # load and run modding toolchain builder
    from main import Main

    tools_main = Main()
    try:
        tools_main.exec()
    except SystemExit:
        # Just catch this so the tool's "sys.exit()" call can properly stop
        # execution
        None

    # end of modding toolchain
    stop_event.set()
    hash_thread.join()


if __name__ == "__main__":
    build_main()