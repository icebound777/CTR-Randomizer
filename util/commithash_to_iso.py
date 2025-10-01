import subprocess

#                       "PRE-"      "ALPH"      "A "
PRE_ALPHA: list[int] = [0x7072652D, 0x616C7068, 0x6120]

def str_to_bytes(str_in):
    byte_array: list[int] = []
    for character in str_in:
        byte_array.append(character.to_bytes())
    return byte_array

def main():
    subprocess_result = subprocess.run(
        ["git", "log", "--oneline", "--abbrev=8", "-n", "1"],
        capture_output=True
    )
    git_commit: str = subprocess_result.stdout.decode(encoding="utf-8")
    git_commit_hash: str = git_commit[:8]
    # print(git_commit_hash)

    with open("../../../build/ctr-u_CTR-Randomizer.bin", "r+b") as in_file:
        in_file.seek(0xE000)
        for _ in range(1000):
            chunk = int.from_bytes(in_file.read(4), "big")
            if chunk == '':
                break
            if chunk == PRE_ALPHA[0]:
                if int.from_bytes(in_file.read(4), "big") == PRE_ALPHA[1]:
                    if int.from_bytes(in_file.read(2), "big") == PRE_ALPHA[2]:
                        in_file.write(str.encode(git_commit_hash, encoding="utf-8"))
                    else:
                        int.from_bytes(in_file.read(2), "big")

if __name__ == "__main__":
    main()
