# CppND-System-Monitor

Uploaded for Troubleshooting.

Current Issue:

Calling Sort in system.cpp causes Floating Point Exception (core dumped) error in Monitor.

<img width="814" alt="Screen Shot 2022-07-17 at 7 06 37 PM" src="https://user-images.githubusercontent.com/61633067/179430439-5db280a4-f40b-45d1-a52e-99c728538a8f.png">


Attempt to fix by implementing 0.0f hardcode at LinuxParser::UpTime, but no change in result.
