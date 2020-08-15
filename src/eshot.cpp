#include <string.h>
#include "common.h"
#include "eshot.h"
#include "rap.h"
#include "gfxapi.h"
#include "glbapi.h"
#include "enemy.h"
#include "fx.h"
#include "shots.h"
#include "objects.h"
#include "anims.h"

int eshotnum, eshothigh;
eshot_t first_eshot, last_eshot;

eshot_t eshots[80];
eshot_t *free_eshot;

texture_t *elaspow[4];

plib_t plib[7];

static int xpos[16] = {
    -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2
};

static int ypos[16] = {
    -3, -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2
};

static int monkeys[6] = {
    1, 2, 3, 4, 5 ,6
};

void ESHOT_Clear(void)
{
    int v1c;
    eshotnum = 0;
    first_eshot.f_0 = NULL;
    first_eshot.f_4 = &last_eshot;
    last_eshot.f_0 = &first_eshot;
    last_eshot.f_4 = NULL;
    free_eshot = &eshots[0];
    memset(eshots, 0, sizeof(eshots));
    for (v1c = 0; v1c < 79; v1c++)
        eshots[v1c].f_4 = &eshots[v1c + 1];
}

eshot_t *ESHOT_Get(void)
{
    eshot_t *v1c;
    if (!free_eshot)
        return NULL;
    eshotnum++;
    if (eshothigh < eshotnum)
        eshothigh = eshotnum;
    v1c = free_eshot;
    free_eshot = free_eshot->f_4;
    memset(v1c, 0, sizeof(eshot_t));
    v1c->f_4 = &last_eshot;
    v1c->f_0 = last_eshot.f_0;
    last_eshot.f_0 = v1c;
    v1c->f_0->f_4 = v1c;
    return v1c;
}

eshot_t *ESHOT_Remove(eshot_t *a1)
{
    eshot_t *v1c;
    eshotnum--;
    v1c = a1->f_0;
    a1->f_4->f_0 = a1->f_0;
    a1->f_0->f_4 = a1->f_4;
    memset(a1, 0, sizeof(eshot_t));
    a1->f_4 = free_eshot;
    free_eshot = a1;
    return v1c;
}

void ESHOT_Init(void)
{
    plib_t *v1c;
    texture_t *v24;
    int v20;
    for (v20 = 0; v20 < 4; v20++)
        elaspow[v20] = (texture_t*)GLB_LockItem(0x10245 + v20);
    ESHOT_Clear();
    memset(plib, 0, sizeof(plib));
    v1c = &plib[0];
    v1c->f_40 = 2;
    v1c->f_0 = 0x101d6;
    v1c->f_2c = 2;
    v1c->f_30 = 0;
    v1c->f_34 = 6;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[1];
    v1c->f_40 = 1;
    v1c->f_0 = 0x101d6;
    v1c->f_2c = 2;
    v1c->f_30 = 0;
    v1c->f_34 = 6;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[2];
    v1c->f_40 = 4;
    v1c->f_0 = 0x101d4;
    v1c->f_2c = 2;
    v1c->f_30 = 1;
    v1c->f_34 = 10;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[4];
    v1c->f_40 = 0x10;
    v1c->f_0 = 0x101d1;
    v1c->f_2c = 2;
    v1c->f_30 = 0;
    v1c->f_34 = 0;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[3];
    v1c->f_40 = 0xc;
    v1c->f_0 = 0x10241;
    v1c->f_2c = 4;
    v1c->f_30 = 0;
    v1c->f_34 = 6;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[5];
    v1c->f_40 = 0xf;
    v1c->f_0 = 0x10251;
    v1c->f_2c = 1;
    v1c->f_30 = 0;
    v1c->f_34 = 10;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;

    v1c = &plib[6];
    v1c->f_40 = 1;
    v1c->f_0 = 0x1024d;
    v1c->f_2c = 4;
    v1c->f_30 = 0;
    v1c->f_34 = 6;
    for (v20 = 0; v20 < v1c->f_2c; v20++)
        v1c->f_4[v20] = (texture_t*)GLB_LockItem(v1c->f_0 + v20);
    v24 = v1c->f_4[0];
    v1c->f_38 = v24->f_c >> 1;
    v1c->f_3c = v24->f_10 >> 1;
}

void ESHOT_Shoot(enemy_t *a1, int a2)
{
    int v1c;
    int v20;
    int v2c;
    eshot_t *v18;
    v1c = a1->f_18 + a1->f_c->f_138[a2];
    v20 = a1->f_1c + a1->f_c->f_168[a2];
    if ((v1c >= 0 || v1c < 320) && (v20 >= 0 || v20 < 200))
    {
        v18 = ESHOT_Get();
        if (!v18)
            return;
        v18->f_18.f_0 = v1c;
        v18->f_18.f_4 = v20;
        v18->f_5c = a1;
        v18->f_60 = a2;
        v2c = a1->f_c->f_78[a2];
        switch (v2c)
        {
        default:
            EXIT_Error("ESHOT_Shoot() - Invalid EShot type");
            break;
        case 0:
            SND_3DPatch(28, v1c, v20);
            v18->f_48 = &plib[1];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_4 -= v18->f_48->f_3c;
            v18->f_18.f_8 = player_cx;
            v18->f_18.f_c = player_cy;
            v18->f_50 = 1;
            v18->f_58 = 0;
            break;
        case 1:
            SND_3DPatch(28, v1c, v20);
            v18->f_48 = &plib[0];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_4 -= v18->f_48->f_3c;
            v18->f_18.f_8 = v18->f_18.f_0;
            v18->f_18.f_c = 200;
            v18->f_50 = v18->f_48->f_34 >> 1;
            v18->f_58 = 1;
            break;
        case 2:
            SND_3DPatch(28, v1c, v20);
            v18->f_48 = &plib[0];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_4 -= v18->f_48->f_3c;
            v18->f_18.f_8 = v18->f_18.f_0 - 32;
            v18->f_18.f_c = v18->f_18.f_4 + 32;
            v18->f_50 = v18->f_48->f_34 >> 1;
            v18->f_58 = 2;
            break;
        case 3:
            SND_3DPatch(28, v1c, v20);
            v18->f_48 = &plib[0];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_4 -= v18->f_48->f_3c;
            v18->f_18.f_8 = v18->f_18.f_0 + 32;
            v18->f_18.f_c = v18->f_18.f_4 + 32;
            v18->f_50 = v18->f_48->f_34 >> 1;
            v18->f_58 = 3;
            break;
        case 4:
            SND_3DPatch(30, v1c, v20);
            v18->f_48 = &plib[2];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_8 = v18->f_18.f_0;
            v18->f_18.f_c = 200;
            v18->f_50 = a1->f_b4 + 1;
            v18->f_58 = 4;
            break;
        case 5:
            SND_3DPatch(29, v1c, v20);
            v18->f_48 = &plib[3];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_8 = v18->f_18.f_0;
            v18->f_18.f_c = 200;
            v18->f_50 = a1->f_b4;
            v18->f_58 = 5;
            break;
        case 6:
            SND_3DPatch(28, v1c, v20);
            v18->f_48 = &plib[4];
            v18->f_10 = v18->f_18.f_0;
            v18->f_14 = v18->f_18.f_4;
            v18->f_18.f_8 = 320;
            v18->f_18.f_c = 200;
            v18->f_50 = 150;
            v18->f_54 = wrand() % 16;
            v18->f_58 = 6;
            break;
        case 7:
            SND_3DPatch(31, v1c, v20);
            v18->f_48 = &plib[5];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_8 = v18->f_18.f_0;
            v18->f_18.f_c = 200;
            v18->f_50 = 8;
            v18->f_58 = 7;
            break;
        case 8:
            SND_3DPatch(monkeys[wrand() % 6], v1c, v20);
            v18->f_48 = &plib[6];
            v18->f_18.f_0 -= v18->f_48->f_38;
            v18->f_18.f_4 -= v18->f_48->f_3c;
            v18->f_18.f_8 = player_cx;
            v18->f_18.f_c = player_cy;
            v18->f_50 = 1;
            v18->f_58 = 8;
            break;
        }
        InitMobj(&v18->f_18);
        MoveSobj(&v18->f_18, 1);
        if (v18->f_18.f_0 < 0 || v18->f_18.f_0 >= 320)
            v18->f_18.f_28 = 1;
        if (v18->f_18.f_4 < 0 || v18->f_18.f_4 >= 200)
            v18->f_18.f_28 = 1;
        if (v18->f_18.f_28)
            ESHOT_Remove(v18);
    }
}

void ESHOT_Think(void)
{
    eshot_t *v1c;
    plib_t *v20;
    int v24, v28;

    for (v1c = first_eshot.f_4; &last_eshot != v1c; v1c = v1c->f_4)
    {
        v20 = v1c->f_48;
        v1c->f_8 = v20->f_4[v1c->f_c];
        v1c->f_c++;
        switch (v1c->f_58)
        {
        case 5:
            if (v1c->f_c < v20->f_2c)
            {
                v1c->f_10 = v1c->f_5c->f_c->f_138[v1c->f_60] + v1c->f_5c->f_18 - 4;
                v1c->f_14 = v1c->f_5c->f_c->f_168[v1c->f_60] + v1c->f_5c->f_1c;
                v24 = abs(v1c->f_10 - player_cx);
                if (v24 < 16 && v1c->f_14 < player_cy)
                {
                    v1c->f_18.f_c = player_cy + (wrand() % 4) - 2;
                    OBJS_SubEnergy(v20->f_40);
                }
            }
            else
            {
                v1c->f_44 = 1;
            }
            break;
        default:
            if (v1c->f_c >= v20->f_2c)
                v1c->f_c = 0;
            if (v20->f_34)
            {
                v1c->f_10 = v1c->f_18.f_0;
                v1c->f_14 = v1c->f_18.f_4;
                MoveSobj(&v1c->f_18, v1c->f_50);
                if (v1c->f_50 < v20->f_34)
                    v1c->f_50++;
            }
            else
            {
                v1c->f_50--;
                if (v1c->f_50)
                {
                    v1c->f_10 = v1c->f_18.f_0 + xpos[v1c->f_54];
                    v1c->f_14 = v1c->f_18.f_4 + ypos[v1c->f_54];
                    v1c->f_18.f_4++;
                    v1c->f_54++;
                    if (v1c->f_54 >= 16)
                        v1c->f_54 = 0;
                }
                else
                {
                    v1c->f_44 = 1;
                    ANIMS_StartAnim(6, v1c->f_10 + 4, v1c->f_14 + 4);
                }
            }
            if (v1c->f_14 >= 200 || v1c->f_14 < 0)
                v1c->f_44 = 1;
            if (v1c->f_10 >= 320 || v1c->f_10 < 0)
                v1c->f_44 = 1;
            v24 = abs(v1c->f_10 - player_cx);
            v28 = abs(v1c->f_14 - player_cy);
            if (v24 < 16 && v28 < 16)
            {
                ANIMS_StartAnim(6, v1c->f_10, v1c->f_14);
                v1c->f_44 = 1;
                OBJS_SubEnergy(v20->f_40);
            }
            break;
        }
        if (v1c->f_44)
        {
            v1c = ESHOT_Remove(v1c);
            continue;
        }
        v1c->f_4c++;
        if (v20->f_30 && (v1c->f_4c & 1) != 0)
        {
            ANIMS_StartAAnim(12, v1c->f_10 + v20->f_38, v1c->f_14);
        }
    }
}

void ESHOT_Display(void)
{
    eshot_t *v1c;
    int v20, v28;
    texture_t *v24;
    for (v1c = first_eshot.f_4; &last_eshot != v1c; v1c = v1c->f_4)
    {
        if (v1c->f_58 == 5)
        {
            for (v20 = v1c->f_14; v20 < v1c->f_18.f_c; v20 += 3)
                GFX_PutSprite(v1c->f_8, v1c->f_10, v20);
            GFX_PutSprite(elaspow[v1c->f_c - 1], v1c->f_10, v1c->f_14);
            v24 = lashit[v1c->f_c - 1];
            v28 = v1c->f_18.f_c - 8;
            if (v28 > 0 && v28 < 200)
            {
                GFX_PutSprite(v24, v1c->f_10 - (v24->f_c >> 2), v28);
            }
        }
        else
            GFX_PutSprite(v1c->f_8, v1c->f_10, v1c->f_14);
    }
}
