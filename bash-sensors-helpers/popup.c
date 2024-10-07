#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#define BUFFER_SIZE 4096
#define HIGH_TEMP 75
#define CRIT_TEMP 95

typedef struct {
    char name[50];
    int temperature;
} TempEntry;

int main() {
    char buffer[BUFFER_SIZE];
    FILE *fp;
    TempEntry temps[10];
    int temp_count = 0;
    int current_fan_speed = -1;
    fp = popen("sensors -j", "r");
    if (fp == NULL) {
        return 1;
    }
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytes_read == 0) {
        pclose(fp);
        return 1;
    }
    buffer[bytes_read] = '\0';
    pclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        return 1;
    }
    cJSON *coretemp = cJSON_GetObjectItem(json, "coretemp-isa-0000");
    if (coretemp == NULL) {
        cJSON_Delete(json);
        return 1;
    }
    printf(" Core | Temp\n");
    printf("----------+----------\n");
    cJSON *core_entry = NULL;
    cJSON_ArrayForEach(core_entry, coretemp) {
        if (cJSON_IsObject(core_entry)) {
            for (int i = 1; i <= 17; i++) {
                char temp_key[20];
                snprintf(temp_key, sizeof(temp_key), "temp%d_input", i);
                if (strstr(core_entry->string, "core") != NULL) {
                    cJSON *temp_input = cJSON_GetObjectItem(core_entry, temp_key);
                    if (temp_input && cJSON_IsNumber(temp_input)) {
                        snprintf(temps[temp_count].name, sizeof(temps[temp_count].name), "%s", core_entry->string);
                        temps[temp_count].temperature = temp_input->valueint;
                        temp_count++;
                    }
                }
            }
        }
    }
    if (temp_count > 0) {
        for (int i = 0; i < temp_count; i++) {
            printf(" %-8s | %-2d°C        \n", temps[i].name, temps[i].temperature);
        }
    } else {
        printf("No valid temperature data found in coretemp-isa-0000.\n");
    }
    cJSON *asus = cJSON_GetObjectItem(json, "asus-isa-0000");
    if (asus != NULL) {
        cJSON *cpu_fan = cJSON_GetObjectItem(asus, "cpu_fan");
        if (cpu_fan) {
            cJSON *fan_entry = cJSON_GetObjectItem(cpu_fan, "fan1_input");
            if (fan_entry) {
                if (cJSON_IsNumber(fan_entry)) {
                    current_fan_speed = fan_entry->valueint;
                }
            }
        }
    }
    if (current_fan_speed != -1) {
        printf("\n Fan Speed: %d RPM\n", current_fan_speed);
    } else {
        printf("No fan speed data found.\n");
    }
    cJSON_Delete(json);
    return 0;
}