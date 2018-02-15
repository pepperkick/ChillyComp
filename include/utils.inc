GetArrayTrueCount(arrayHandle) {
    int c = 0;

    for(int i = 0; i < GetArraySize(arrayHandle); i++) {
        if(GetArrayCell(arrayHandle, i)) c++;
    }

    return c;
}