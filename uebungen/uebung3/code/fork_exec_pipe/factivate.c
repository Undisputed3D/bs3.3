#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
/*
 * These are all headers you need!
 */

int main(int argc, char *argv[]) {
    /*
     * Do as you were told.
     * Split argv[1..n] by using a <DELIMITER> into:
     * <PROGRAM 1> [<PARAMETER>...]
     * <PROGRAM 2> [<PARAMETER>...]
     */

    /*
     * create first child (fork()) and execute
     * <PROGRAM 1> [<PARAMETER>...] with exec*()
     */

    /*
     * create second child (fork()) and execute
     * <PROGRAM 1> [<PARAMETER>...]  with exec*()
     */

    /*
     * Typically, here is the parent. Make sure this code is called
     * _after_ both subprocesses were launched!
     * Use wait() or waitpid() and wait until _both_ subprocesses are finished!
     * Then use exit()
     */
}
