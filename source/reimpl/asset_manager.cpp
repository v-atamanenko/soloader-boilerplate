#include "reimpl/asset_manager.h"
#include "utils/logger.h"

#include <pthread.h>
#include <malloc.h>
#include <cstring>
#include <cstdio>
#include <libc_bridge/libc_bridge.h>
#include <string>

typedef struct assetManager {
    int dummy = 0; // TODO: mb we will need to store something here in future
    pthread_mutex_t mLock;
} assetManager;

typedef struct aAsset {
    char * filename;
    FILE* f;
    size_t bytesRead;
    size_t fileSize;
} asset;

static AAssetManager * g_AAssetManager = nullptr;

AAssetManager * AAssetManager_create() {
    if (g_AAssetManager) return g_AAssetManager;

    assetManager am;

    pthread_mutex_init(&am.mLock, nullptr);

    g_AAssetManager = (AAssetManager *) malloc(sizeof(assetManager));
    memcpy(g_AAssetManager, &am, sizeof(assetManager));

    return g_AAssetManager;
}

AAsset* AAssetManager_open(AAssetManager* mgr, const char* filename, int mode) {
    std::string realp = std::string(DATA_PATH) + std::string("assets/") + std::string(filename);

    auto * a = new aAsset;
    a->filename = (char *) malloc(realp.length() + 1);
    strcpy(a->filename, realp.c_str());
    a->bytesRead = 0;

#ifdef USE_SCELIBC_IO
    a->f = sceLibcBridge_fopen((const char *)a->filename, "r");
#else
    a->f = fopen((cost char *)a->filename, "r");
#endif

    if (!a->f) {
        free(a->filename);
        delete a;
        a = nullptr;
    } else {
#ifdef USE_SCELIBC_IO
        sceLibcBridge_fseek(a->f, 0, SEEK_END);
        a->fileSize = sceLibcBridge_ftell(a->f);
        sceLibcBridge_fseek(a->f, 0, SEEK_SET);
        sceLibcBridge_fclose(a->f);
#else
        fseek(a->f, 0, SEEK_END);
        a->fileSize = ftell(a->f);
        fseek(a->f, 0, SEEK_SET);
        fclose(a->f);
#endif
    }

    l_debug("[AAssetManager] AAssetManager_open(%p, %s, %i): %p", mgr, realp.c_str(), mode, a);
    return (AAsset *) a;
}

void AAsset_close(AAsset* asset) {
    l_debug("AAsset_close(%p)", asset);

    if (asset) {
        auto * a = (aAsset *) asset;
        free(a->filename);
        fclose(a->f);
        delete a;
    }
}

int AAsset_read(AAsset* asset, void* buf, size_t count) {
    l_debug("AAsset_read(%p, %p, %i)", asset, buf, count);

    if (!asset) {
        return -1;
    }

    auto * a = (aAsset *) asset;

#ifdef USE_SCELIBC_IO
    size_t ret = sceLibcBridge_fread(buf, 1, count, a->f);
#else
    size_t ret = fread(buf, 1, count, a->f);
#endif

    if (ret > 0) {
        a->bytesRead += ret;
        return (int) ret;
    } else {
#ifdef USE_SCELIBC_IO
        if (ret == 0 || sceLibcBridge_feof(a->f)) {
#else
        if (ret == 0 || feof(a->f)) {
#endif
            return 0;
        } else {
            return -1;
        }
    }
}

off_t AAsset_seek(AAsset* asset, off_t offset, int whence) {
    l_debug("AAsset_seek(%p, %d, %i)", asset, offset, whence);

    if (!asset) {
        return (off_t) -1;
    }

    auto * a = (aAsset *) asset;

#ifdef USE_SCELIBC_IO
    auto ret = (off_t) sceLibcBridge_fseek(a->f, offset, whence);
#else
    auto ret = (off_t) fseek(a->f, offset, whence);
#endif

    return ret;
}

off_t AAsset_getRemainingLength(AAsset* asset) {
    l_debug("AAsset_getRemainingLength");
    if (!asset) {
        return (off_t) -1;
    }

    auto * a = (aAsset *) asset;

    return (off_t)(a->fileSize - a->bytesRead);
}

off_t AAsset_getLength(AAsset* asset) {
    l_debug("AAsset_getLength");
    if (!asset) {
        return (off_t) -1;
    }

    auto * a = (aAsset *) asset;

    return (off_t)a->fileSize;
}

AAssetDir* AAssetManager_openDir(AAssetManager* mgr, const char* dirName) {
    l_error("AAssetManager_openDir: %s", dirName);
    return (AAssetDir *)strdup("dummy");
}

void AAssetDir_close(AAssetDir* assetDir) {
    l_error("AAssetDir_close");
    free(assetDir);
}
