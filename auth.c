#include "auth.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> 
#include <stdlib.h>

void hash_password(const char *password, char *hashed_password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, strlen(password), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[SHA256_DIGEST_LENGTH * 2] = '\0';
}

bool username_exists(const char *username)
{
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (!file)
        return false;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char stored_username[50];
        sscanf(line, "%[^|]", stored_username);
        if (strcmp(stored_username, username) == 0)
        {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

bool register_account(const char *username, const char *password)
{
    if (username_exists(username))
    {
        return false; 
    }

    char hashed_password[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(password, hashed_password);

    FILE *file = fopen(USER_DATA_FILE, "a");
    if (!file)
        return false;

    fprintf(file, "%s|%s|1000\n", username, hashed_password); 
    fclose(file);
    return true;
}

bool login_account(const char *username, const char *password, char *session_id)
{
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (!file)
        return false;

    char line[256];
    char hashed_input_password[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_password(password, hashed_input_password);

    while (fgets(line, sizeof(line), file))
    {
        char stored_username[50], stored_password_hash[65];
        sscanf(line, "%[^|]|%[^|]", stored_username, stored_password_hash);
        if (strcmp(stored_username, username) == 0 && strcmp(stored_password_hash, hashed_input_password) == 0)
        {
            snprintf(session_id, 20, "%s_session", username); 
            fclose(file);
            return true; 
        }
    }

    fclose(file);
    return false; 
}
