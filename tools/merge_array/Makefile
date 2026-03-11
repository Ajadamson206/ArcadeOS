CC := gcc

WD := .

CFILES := $(WD)/merge.c

OUT := $(WD)/merge

.PHONY: merge_array

$(OUT): $(CFILES)
	$(CC) $^ -o $@

merge_array: $(OUT)