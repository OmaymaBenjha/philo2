import subprocess
import sys

# --- Configuration ---
NUM_TESTS = 50
PHILO_EXECUTABLE = "./philo"
ARGS = ["5", "800", "200", "200", "7"]

def run_single_test(test_number):
    """
    Runs a single instance of the philo program and checks its output.
    Returns a tuple: (result_code, message)
    Result codes:
        0: Success (no death)
        1: Failure (death occurred)
        2: Failure (program error/crash)
    """
    command = [PHILO_EXECUTABLE] + ARGS
    try:
        # Run the command, capture output, set a timeout
        # A timeout is crucial to prevent the script from hanging forever
        # if the philosophers deadlock. 15 seconds should be more than enough.
        process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            timeout=15,
            check=False # Do not raise exception on non-zero exit codes
        )

        output = process.stdout
        stderr = process.stderr

        # 1. Check for program errors or crashes
        if process.returncode != 0:
            error_message = (
                f"Test #{test_number}: FAILED with program error!\n"
                f"  Exit Code: {process.returncode}\n"
                f"  Stderr: {stderr.strip() if stderr else 'None'}\n"
                f"  Stdout: {output.strip() if output else 'None'}"
            )
            return (2, error_message)

        # 2. Check for the word "died" in the output
        if "died" in output:
            death_message = (
                f"Test #{test_number}: FAILED - A philosopher died!\n"
                f"  Output containing 'died':\n"
            )
            # Find and show the lines with "died"
            lines_with_death = [line for line in output.split('\n') if "died" in line]
            death_message += "\n".join(f"    > {line}" for line in lines_with_death)
            return (1, death_message)
        
        # 3. If we reach here, the test passed
        return (0, f"Test #{test_number}: PASSED")

    except FileNotFoundError:
        print(f"Error: The executable '{PHILO_EXECUTABLE}' was not found.")
        print("Please compile your project first using 'make'.")
        sys.exit(1)
    except subprocess.TimeoutExpired:
        timeout_message = (
            f"Test #{test_number}: FAILED - Program timed out (possible deadlock)!\n"
            f"  The program ran for more than 15 seconds and was terminated."
        )
        return (2, timeout_message)
    except Exception as e:
        exception_message = f"An unexpected error occurred: {e}"
        return (2, exception_message)


def main():
    """Main function to run all tests and print a summary."""
    print(f"--- Starting Philosopher Test Suite ---")
    print(f"Running '{' '.join([PHILO_EXECUTABLE] + ARGS)}' {NUM_TESTS} times...")
    
    success_count = 0
    death_count = 0
    error_count = 0
    
    failed_messages = []

    for i in range(1, NUM_TESTS + 1):
        result_code, message = run_single_test(i)
        
        if result_code == 0:
            success_count += 1
            sys.stdout.write('.')
        elif result_code == 1:
            death_count += 1
            sys.stdout.write('D') # D for Death
            failed_messages.append(message)
        else: # result_code == 2
            error_count += 1
            sys.stdout.write('E') # E for Error
            failed_messages.append(message)
        
        # Flush the output to show progress immediately
        sys.stdout.flush()

    print("\n\n--- Test Summary ---")
    print(f"Total Tests: {NUM_TESTS}")
    print(f"  \033[92mPassed: {success_count}\033[0m") # Green color
    print(f"  \033[91mFailed (Deaths): {death_count}\033[0m") # Red color
    print(f"  \033[93mFailed (Errors/Crashes): {error_count}\033[0m") # Yellow color
    
    if failed_messages:
        print("\n--- Failure Details ---")
        for msg in failed_messages:
            print(msg)
            print("-" * 20)

    if death_count == 0 and error_count == 0:
        print("\n\033[1;92mCongratulations! All tests passed without any deaths or errors.\033[0m")
    else:
        print("\n\033[1;91mSome tests failed. Please review the details above.\033[0m")


if __name__ == "__main__":
    main()