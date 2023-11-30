__import__("warnings").filterwarnings("ignore", category=DeprecationWarning)


if __name__ == "__main__":
    file_path = __import__("os").path.abspath(
        input("Enter the path to the .pyc file: "))

    if not file_path.endswith(".pyc"):
        print("The file is not a .pyc file.")
        exit(1)

    with open(file_path, "rb") as file:
        magic_number = file.read(4).hex().upper().strip()
    print(f"Found magic number: 0x{magic_number}")

    new_py = __import__('importlib').util.MAGIC_NUMBER.hex().upper()
    old_py = __import__('imp').get_magic().hex().upper()

    print(f"| 0x{new_py} is for Python 3.4+")
    print(f"| 0x{old_py} is for Python <3.4")

    if magic_number == new_py:
        print(f"The file {file_path} is a Python version 3.4+ compiled file.")
    elif magic_number == old_py:
        print(f"The file {file_path} is a Python version <3.4 compiled file.")
    else:
        print(f"The file {file_path} is not a .pyc file.")
