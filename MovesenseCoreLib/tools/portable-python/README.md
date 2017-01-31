# Portable Python and friends

This repository contains portable version of Python 2.7 and all the Python packages required to run test automation.

Readily installed packages are:

 * [Robot Framework](http://robotframework.org/)
 * [PyYaml](http://pyyaml.org/)
 * [Websocket-client](https://github.com/liris/websocket-client)
 * [Paramiko](https://github.com/paramiko/paramiko)
 * [pywin32](https://pypi.python.org/pypi/pywin32)
 * [Pillow](https://github.com/python-pillow/Pillow)
 * [Selenium2Library](https://github.com/robotframework/Selenium2Library/wiki)
 * [psutil](https://pypi.python.org/pypi/psutil)
 * [enum34](https://pypi.python.org/pypi/enum34)

If there is a need for other packages as well, please `pip install` those here (remember to user `\App\pip.exe` and not your system pip) and commit the changes.

If `\App\pip.exe` doesn't work then install pip by running easy_install.exe in App\Scripts folder (`easy_install.exe pip`).

## Path variables

For more comfort, put `path_to_here\App` and `path_to_here\App\Scripts` in your PATH system variable. If you do not want to do that, just open shell in here and call `set_path_variables.bat` and it will set those for you for the current session. Once you close that shell, those are gone.

## To run the tests

Your tests should be in the same repo as the system under test is. Also you need to `git clone ssh://git@altssh.bitbucket.org:443/suunto/sds.git` and follow the instructions of the README in that repo.

Then call `pybot path/to/test/directory` and it will run all the tests in there. For more detailed instruction on how to use pybot R(obot Framework test runner), please consult the official documentation at http://robotframework.org/#documentation and especially the user guide at http://robotframework.org/robotframework/latest/RobotFrameworkUserGuide.html

## Or just install Python by yourself

You can of course install Python 2.7 the normal way. In that case just check this README for the Python libs you need to `pip install` on your machine.