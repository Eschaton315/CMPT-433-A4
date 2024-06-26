#include "hal/sharedUtil.h"



//runCommand function taken from assignment page
void runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
    if (fgets(buffer, sizeof(buffer), pipe) == NULL)
        break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

//getTimeInMs function copied from assignment page
long long getTimeInMs()
{
struct timespec spec;
clock_gettime(CLOCK_REALTIME, &spec);
long long seconds = spec.tv_sec;
long long nanoSeconds = spec.tv_nsec;
long long milliSeconds = seconds * 1000
+ nanoSeconds / 1000000;
return milliSeconds;
}

//sleepForMs function copied from assignemnt page
void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}


//echo to file for editing their values
void EchoToFile(char* filePath, char* contents){
	// Open direction file
	int file = open(filePath, O_WRONLY);
	if (file < 0) {
		printf("Failed to read file %s", filePath);
		return;
	}

	write(file, contents, strlen(contents));
	// Close
	close(file);

	return;
}

void writeToFile(char* fileName, char* value) {
    FILE* pfile = fopen(fileName, "w");
    if (pfile == NULL) {
        printf("ERROR: Unable to open export file.\n");
        exit(1);
    }
    fprintf(pfile, "%s", value);
    // Write to data to the file using fprintf():
    fclose(pfile);
}
