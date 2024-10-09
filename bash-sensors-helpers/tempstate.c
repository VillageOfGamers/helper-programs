#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#define BUFFER_SIZE 4096
#define HIGH_TEMP 75
#define CRIT_TEMP 85

typedef struct {
    char name[50];
    int temperature;
} TempEntry;

int main() {
    int current_temp;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    fp = popen("sensors -j", "r");
    if (fp == NULL) {
        printf("Failed to run sensors command\n");
        return 1;
    }
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytes_read == 0) {
        printf("No data read from sensors\n");
        pclose(fp);
        return 1;
    }
    buffer[bytes_read] = '\0';
    pclose(fp);
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }
    cJSON *sensor = NULL;
    cJSON_ArrayForEach(sensor, json) {
        cJSON *core = NULL;
        cJSON_ArrayForEach(core, sensor) {
            cJSON *temp_entry = cJSON_GetObjectItem(core, "temp1_input");
            if (cJSON_IsNumber(temp_entry)) {
                current_temp = temp_entry->valueint;
            }
        }
    }
    if (current_temp >= CRIT_TEMP) {
        printf("CRITICAL!\n");
    } else if (current_temp >= HIGH_TEMP) {
        printf("High\n");
    } else {
        printf("Normal\n");
    }
    cJSON_Delete(json);
    return 0;
}