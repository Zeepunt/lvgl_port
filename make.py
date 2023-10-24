import sys
import subprocess

def config():
    subprocess.run(["cmake", "-G", "Ninja", "-B", "build", "-D", "CMAKE_BUILD_TYPE=Debug"])

def build():
    subprocess.run(["cmake", "--build", "build"])

def run():
    subprocess.run(["./build/lvgl_simulator"])

def print_usage():
    print("""
    Usage:
        python make.py     -- cmake config and build
        python make.py run -- run executable file
    """)

def shell():
    if (len(sys.argv) > 1):
        if (sys.argv[1] == "run"):
            run()
        else:
            print_usage()
    else:
        config()
        build()

if __name__ == "__main__":
    shell()