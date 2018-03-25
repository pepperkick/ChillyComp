GetArrayTrueCount(Handle arrayHandle) {
    int c = 0;

    for(int i = 0; i < GetArraySize(arrayHandle); i++) {
        if(GetArrayCell(arrayHandle, i)) c++;
    }

    return c;
}

public bool:IsInteger(String:buffer[])
{
    new len = strlen(buffer);
    for (new i = 0; i < len; i++) {
        if (!IsCharNumeric(buffer[i]))
            return false;
    }

    return true;
}