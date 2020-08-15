#include <stdio.h>
#include <string.h>
#include <io.h>
#include "common.h"
#include "prefapi.h"

static int FUN_0002f210(const char *a1, const char *a2, const char *a3, char *a4, int a5, const char *a6)
{
    char *va;
    char v9c[128];
    int l;
    short v10, i;
    FILE *vs;

    v10 = 0;
    vs = fopen(a6, "r");
    if (vs)
    {
        l = strlen(a1);

        while (!v10 && fgets(v9c, 128, vs) != 0)
        {
            if (v9c[0] == '[')
            {
                for (i = 0; v9c[i] != 0 && v9c[i] != ']'; i++) {
                }
                if (v9c[i] == ']')
                {
                    v9c[i] = '\0';
                    if (!stricmp(v9c + 1, a1))
                        v10 = 1;
                }
            }
        }

        while (v10 && fgets(v9c, 128, vs) != 0)
        {
            if (v9c[0] == '[')
            {
                v10 = 0;
                continue;
            }
            va = strtok(v9c, "=\n");
            if (va)
            {
                if (!stricmp(va, a2))
                {
                    fclose(vs);
                    va = strtok(NULL, "=\n");
                    if (va)
                    {
                        strncpy(a4, va, a5);
                        a4[a5-1] = '\0';
                    }
                    else
                        a4[0] = '\0';
                    return 1;
                }
            }
        }
        fclose(vs);
    }
    if (a3)
        strncpy(a4, a3, a5);
    else
        a4[0] = '\0';
    return 0;
}

static int FUN_0002f3b4(const char *a1, const char *a2, const char *a3, const char *a4)
{
    char *va;
    char va8[128];
    FILE *vs;
    short vc, i;
    int v28, vbp, vd, v10, l, v1c, v18, vdi, v20;

    vc = 0;
    if (!a1 || !*a1)
        return 0;
    vs = fopen(a4, "rb+");
    if (!vs)
    {
        vs = fopen(a4, "wb+");
        if (!vs)
            return 0;
        fseek(vs, 0, SEEK_SET);
    }
    v10 = 0;

    while (!vc && fgets(va8, 128, vs) != 0)
    {
        if (va8[0] == '\r' || va8[0] == '\n')
            continue;
        if (va8[0] == '[')
        {
            for (i = 0; va8[i] != 0 && va8[i] != ']'; i++) {
            }
            if (va8[i] == ']')
            {
                va8[i] = '\0';
                if (!stricmp(va8 + 1, a1))
                {
                    vc = 1;
                    break;
                }
            }
            else
                continue;
        }
        v10 = ftell(vs);
    }
    if (!vc)
    {
        fseek(vs, 0, SEEK_END);
        if (!a2 || !*a2)
        {
            fclose(vs);
            return 1;
        }
        fprintf(vs, "\r\n[%s]\r\n", a1);
        v28 = vbp = ftell(vs);
    }
    else
    {
        v28 = ftell(vs);
        while (fgets(va8, 128, vs) != 0)
        {
            vbp = ftell(vs);
            if (va8[0] == '[')
            {
                vc = 0;
                break;
            }
            va = strtok(va8, "=\r\n");
            if (va)
            {
                if (!stricmp(va, a2))
                {
                    va = strtok(NULL, "=\r\n");
                    if (!stricmp(a3, va))
                    {
                        fclose(vs);
                        return 1;
                    }
                    vc = 1;
                    break;
                }
                else
                    v28 = vbp;
            }
        }
    }
    if (!vc)
    {
        if (vbp == v28)
        {
            if (!a2 || !*a2 || !a3 || !*a3)
            {
                fclose(vs);
                return 1;
            }
            fseek(vs, 0, SEEK_END);
            fprintf(vs, "%s=%s\r\n", a2, a3);
            fclose(vs);
            return 1;
        }
        else
        {
            if (a2 && *a2)
                vd = 0;
            else
            {
                vd = v28 - v10;
                vbp = v28;
            }
        }
    }
    else
    {
        vd = vbp - v28;
    }
    if (a2 && *a2 && a3 && *a3)
    {
        sprintf(va8, "%s=%s\r\n", a2, a3);
        l = strlen(va8);
    }
    else
        l = 0;
    v1c = l - vd;
    if (a2 && *a2)
        v10 = v28;
    if (v1c < 0)
    {
        while (1)
        {
            fseek(vs, vbp, SEEK_SET);
            vdi = fread(va8, 1, 128, vs);
            if (!vdi)
                break;
            v18 = ftell(vs);
            fseek(vs, v1c + vbp, SEEK_SET);
            vbp = v18;
            fwrite(va8, 1, vdi, vs);
        }
        fseek(vs, v1c, SEEK_END);
        _chsize(fileno(vs), ftell(vs));
    }
    else if (v1c > 0)
    {
        fseek(vs, 0, SEEK_END);
        v20  = ftell(vs);
        vbp = 128;
        while (1)
        {
            int vd1;
            vd1 = v20 - vbp;
            if (vd1 < v10)
            {
                vbp = v20 - v10;
                if (vbp <= 0)
                    break;
                vd1 = v10;
            }
            v20 = vd1;
            fseek(vs, v20, SEEK_SET);
            vdi = fread(va8, 1, vbp, vs);
            if (!vdi)
                break;
            fseek(vs, v1c + v20, SEEK_SET);
            fwrite(va8, 1, vdi, vs);
            fflush(vs);
        }
    }
    if (a2 && *a2 && a3 && *a3)
    {
        fseek(vs, v10, SEEK_SET);
        fprintf(vs, "%s=%s\r\n", a2, a3);
    }
    fclose(vs);
    return 1;
}

static char preference[_MAX_PATH];

int INI_InitPreference(const char *a1)
{
    if (a1)
        strcpy(preference, a1);
    return !access(preference, 4);
}

int INI_GetPreferenceLong(const char *a1, const char *a2, int a3)
{
    char s1[20], s2[20];
    ltoa(a3, s1, 10);
    INI_GetPreference(a1, a2, s2, 20, s1);
    return atol(s2);
}

int INI_GetPreferenceHex(const char *a1, const char *a2, int a3)
{
    char s1[32], s2[32], t;
    ltoa(a3, s1, 10);
    INI_GetPreference(a1, a2, s2, 32, s1);
    sscanf(s2, "%x", &t);
    return t;
}

int INI_GetPreferenceBool(const char *a1, const char *a2, short a3)
{
    char s2[10];
    char *d;
    if (a3)
        INI_GetPreference(a1, a2, s2, 10, "TRUE");
    else
        INI_GetPreference(a1, a2, s2, 10, "FALSE");
    if (s2[0] == '1' || !stricmp(s2, "TRUE"))
        return 1;
    if (s2[0] == '0' || !stricmp(s2, "FALSE"))
        return 0;
    return atoi(s2);
}

char *INI_GetPreference(const char *a1, const char *a2, char *a3, int a4, const char *a5)
{
    if (!a1 || !a2 || !a3)
    {
        if (a3)
            *a3 = 0;
        return NULL;
    }
    FUN_0002f210(a1, a2, a5, a3, a4, preference);
    return a3;
}

int INI_PutPreferenceLong(const char *a1, const char *a2, int a3)
{
    char s1[32];
    if (a3 == -1)
    {
        INI_DeletePreference(a1, a2);
        return 0;
    }
    ltoa(a3, s1, 10);
    return INI_PutPreference(a1, a2, s1);
}

int INI_PutPreferenceHex(const char *a1, const char *a2, int a3)
{
    char s1[36];
    if (a3 == -1)
    {
        INI_DeletePreference(a1, a2);
        return 0;
    }
    sprintf(s1, "%x", a3);
    return INI_PutPreference(a1, a2, s1);
}

int INI_PutPreferenceBool(const char *a1, const char *a2, short a3)
{
    if (a3)
        return INI_PutPreference(a1, a2, "TRUE");
    return INI_PutPreference(a1, a2, "FALSE");
}

int INI_PutPreference(const char *a1, const char *a2, const char *a3)
{
    if (!a1 || !a2 || !a3)
        return 0;
    return FUN_0002f3b4(a1, a2, a3, preference);
}

int INI_DeletePreference(const char *a1, const char *a2)
{
    if (!a1)
        return 0;
    FUN_0002f3b4(a1, a2, NULL, preference);
}
