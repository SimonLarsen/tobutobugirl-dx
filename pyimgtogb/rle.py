RLE_MAX_RUN = 255

def compress(data):
    if len(data) == 0:
        return []

    value = data[0]
    run, i = 1, 1
    out = []

    while i < len(data):
        if data[i] == data[i-1] and run < RLE_MAX_RUN:
            run = run + 1
        else:
            out.append(data[i-1])
            if run > 1:
                out.append(data[i-1])
                out.append(run)
            run = 1
        i = i + 1

    out.append(data[-1])
    if run > 1:
        out.append(data[-1])
        out.append(run)

    return out


