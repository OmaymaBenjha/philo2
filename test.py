#!/usr/bin/env python3

import subprocess
import os
import sys
from enum import Enum

# --- Configuration ---
EXECUTABLE = "./philo"
TIMEOUT_SECONDS = 10 # How long to wait before killing a test

# --- ANSI Colors for beautiful output ---
class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    ENDC = '\033[0m'

# --- Define the expected outcomes for tests ---
class ExpectedResult(Enum):
    NO_DEATH = "Should run without death"
    DEATH = "Exactly one philosopher should die"
    MEALS_DONE = "Should finish by meal count without death"
    IMMEDIATE_EXIT = "Should exit immediately without actions"

# --- The Test Suite ---
# Each test is a dictionary with a name, arguments, and an expected result.
test_cases = [
    # --- No Death Scenarios ---
    {
        "name": "Correctness: 4 800 200 200 (Classic Even)",
        "args": ["4", "800", "200", "200"],
        "expected": ExpectedResult.NO_DEATH
    },
    {
        "name": "Correctness: 5 800 200 200 (Classic Odd)",
        "args": ["5", "800", "200", "200"],
        "expected": ExpectedResult.NO_DEATH
    },
    {
        "name": "Correctness: 4 400 200 200 (Tight but safe)",
        "args": ["4", "400", "200", "200"],
        "expected": ExpectedResult.NO_DEATH
    },

    # --- Guaranteed Death Scenarios ---
    {
        "name": "Death Logic: 4 310 200 100 (Starvation)",
        "args": ["4", "310", "200", "100"],
        "expected": ExpectedResult.DEATH
    },
    {
        "name": "Death Logic: 5 400 200 200 (Odd philo death)",
        "args": ["5", "400", "200", "200"],
        "expected": ExpectedResult.DEATH
    },
    {
        "name": "Death Logic: 2 200 100 100 (Fast starvation)",
        "args": ["2", "200", "100", "100"],
        "expected": ExpectedResult.DEATH
    },

    # --- Meal Count Scenarios ---
    {
        "name": "Meal Count: 5 800 200 200 7 (Stops on meals)",
        "args": ["5", "800", "200", "200", "7"],
        "expected": ExpectedResult.MEALS_DONE
    },
    {
        "name": "Meal Count: 4 800 200 200 1 (Stops after one meal)",
        "args": ["4", "800", "200", "200", "1"],
        "expected": ExpectedResult.MEALS_DONE
    },
    {
        "name": "Meal Count: 4 310 200 100 10 (Dies before meals)",
        "args": ["4", "310", "200", "100", "10"],
        "expected": ExpectedResult.DEATH
    },

    # --- Edge Cases ---
    {
        "name": "Edge Case: 1 800 200 200 (The Lone Philosopher)",
        "args": ["1", "800", "200", "200"],
        "expected": ExpectedResult.DEATH
    },
    {
        "name": "Edge Case: 5 800 200 200 0 (Zero meals)",
        "args": ["5", "800", "200", "200", "0"],
        "expected": ExpectedResult.IMMEDIATE_EXIT
    },
    {
        "name": "Edge Case: 200 1000 300 300 (Max philos)",
        "args": ["200", "1000", "300", "300"],
        "expected": ExpectedResult.NO_DEATH
    }
]

def run_test(test):
    """Runs a single test case and returns (passed, reason)."""
    command = [EXECUTABLE] + test["args"]
    expected = test["expected"]
    
    try:
        # Run the executable, capture its output, and enforce a timeout
        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            timeout=TIMEOUT_SECONDS
        )
        
        output = result.stdout
        death_count = output.count("died")
        
        # --- Validate the result based on expectations ---
        if expected == ExpectedResult.NO_DEATH:
            if death_count == 0:
                return (True, "Test was killed after timeout with no deaths, as expected.")
            else:
                return (False, f"Expected NO DEATH, but found {death_count} death(s).")

        elif expected == ExpectedResult.DEATH:
            if death_count == 1:
                return (True, "Correctly detected 1 death.")
            elif death_count > 1:
                return (False, f"Expected 1 death, but found {death_count}. (Simulation should stop after first death)")
            else:
                return (False, "Expected 1 death, but found none.")

        elif expected == ExpectedResult.MEALS_DONE:
            if death_count == 0:
                return (True, "Finished by meal count without any deaths.")
            else:
                return (False, f"Expected to finish by meal count, but a death occurred ({death_count} found).")

        elif expected == ExpectedResult.IMMEDIATE_EXIT:
            if death_count == 0 and len(output.splitlines()) < 5: # Should exit very quickly
                 return (True, "Exited immediately as expected.")
            else:
                 return (False, "Did not exit immediately or printed too much output.")

    except subprocess.TimeoutExpired:
        # The process was killed because it ran for too long
        output = ""
        try:
            output = str(sys.exc_info()[1].stdout)
        except:
            pass
        
        death_count = output.count("died")

        if expected in [ExpectedResult.DEATH, ExpectedResult.MEALS_DONE, ExpectedResult.IMMEDIATE_EXIT]:
            return (False, f"Test timed out after {TIMEOUT_SECONDS}s. It should have finished.")
        elif expected == ExpectedResult.NO_DEATH:
            if death_count == 0:
                return (True, f"Test ran for {TIMEOUT_SECONDS}s with no deaths, as expected.")
            else:
                return (False, f"Expected NO DEATH, but found {death_count} death(s) before timeout.")

    except FileNotFoundError:
        print(f"{Colors.RED}Error: Executable '{EXECUTABLE}' not found.{Colors.ENDC}")
        print("Please compile your project with 'make' first.")
        sys.exit(1)
        
    except Exception as e:
        return (False, f"An unexpected error occurred: {e}")

    return (False, "An unknown validation error occurred.")

def main():
    """Main function to run all tests."""
    if not os.path.exists(EXECUTABLE):
        print(f"{Colors.RED}Error: Executable '{EXECUTABLE}' not found.{Colors.ENDC}")
        print("Please compile your project with 'make' first.")
        return

    print(f"{Colors.BLUE}--- Starting Philosopher Test Suite ---{Colors.ENDC}")
    
    passed_count = 0
    total_tests = len(test_cases)
    
    for i, test in enumerate(test_cases):
        print(f"\n{Colors.YELLOW}[{i+1}/{total_tests}] Running Test: {test['name']}{Colors.ENDC}")
        print(f"    Command: {EXECUTABLE} {' '.join(test['args'])}")
        print(f"    Expected: {test['expected'].value}")

        passed, reason = run_test(test)

        if passed:
            passed_count += 1
            print(f"    {Colors.GREEN}Result: [PASS]{Colors.ENDC} - {reason}")
        else:
            print(f"    {Colors.RED}Result: [FAIL]{Colors.ENDC} - {reason}")

    print(f"\n{Colors.BLUE}--- Test Suite Finished ---{Colors.ENDC}")
    if passed_count == total_tests:
        print(f"{Colors.GREEN}Final Result: All {passed_count} / {total_tests} tests passed! Congratulations!{Colors.ENDC}")
    else:
        print(f"{Colors.RED}Final Result: {passed_count} / {total_tests} tests passed.{Colors.ENDC}")

if __name__ == "__main__":
    main()