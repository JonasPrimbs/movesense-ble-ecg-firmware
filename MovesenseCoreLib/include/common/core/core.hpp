#pragma once

/**
 * Setup the low level system operations.
 *
 * If minimumInit passed, simulator build will not initialize simulated drivers.
 * Other parameters passed to the underlying bsl::init if platform supports it.
 */
void initializeSystem(bool minimumInit, int argc, char* argv[]);

/**
 * Start and keep executing the system kernel (threads start)
 * This function does not return on target.
 */
void runSystemKernel(const bool runDisplayLoop);
