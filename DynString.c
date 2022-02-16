//
// Created by mbart on 16/02/2022.
//

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "DynString.h"


void ds__reset(DynString* out) {
    out->ds_string = 0;
    out->ds_size = 0;
    out->ds_capacity = 0;
}


DynString ds__with_size(size_t size) {
    return (DynString) {
        .ds_string = malloc(size * sizeof(char)),
        .ds_capacity = size,
        .ds_size = size,
    };
}


DynString ds__move(DynString* in) {
    DynString out = {
            .ds_string = in->ds_string,
            .ds_capacity = in->ds_capacity,
            .ds_size = in->ds_size,
    };
    ds__reset(in);
    return out;
}


DynString ds__grow(DynString* in) {
    in->ds_capacity *= 2;
    in->ds_string = realloc(in->ds_string, in->ds_capacity);
    return ds__move(in);
}


DynString ds_new() {
    return (DynString) { 0 };
}


DynString ds_with_capacity(size_t capacity) {
    return (DynString) {
        .ds_string = malloc(capacity * sizeof(char)),
        .ds_capacity = capacity
    };
}


DynString ds_from_string(char const cstr[static 1]) {
    size_t size = strlen(cstr) + 1;
    char* strn = malloc(size * sizeof(char));

    strcpy_s(strn, size, cstr);

    return (DynString) {
        .ds_string = strn,
        .ds_capacity = size,
        .ds_size = size,
    };
}


DynString ds_copy_ds(DynString ds_copy) {
    return ds_from_string(ds_string(ds_copy));
}


DynString ds_concat(DynString front, DynString tail) {

    // Sum of both minus one null-character
    size_t sum_size = ds_size(front) + ds_size(tail) - 1;
    DynString sum_ds = ds__with_size(sum_size);

    strcpy_s(
            sum_ds.ds_string,
            ds_size(front),
            ds_string(front));

    // Start after front minus null-character
    strcpy_s(
            sum_ds.ds_string + ds_size(front) - 1,
            ds_size(tail),
            ds_string(tail));

    return sum_ds;
}

DynString ds_combine(DynString* front, DynString* tail) {
    DynString output = ds_concat(*front, *tail);
    ds_free(front);
    ds_free(tail);

    return output;
}

DynString ds_splice(DynString* source, DynString* insert, size_t index) {
    // Split source
    DynStringPair pair = ds_split(source, index);

    // Then merge the three pieces
    DynString output;
    output = ds_combine(&pair.front, insert);
    output = ds_combine(&output, &pair.tail);

    return output;
}


DynStringPair ds_split(DynString* input, size_t index) {
    assert(index < input->ds_size);

    size_t tail_size = input->ds_size - index;

    DynStringPair pair = {
            .front = ds__move(input),
            .tail = ds__with_size(tail_size),
    };

    // Copy tail from index until end
    strcpy_s(pair.tail.ds_string, tail_size, pair.front.ds_string + index);

    // Reduce front and add null-character
    pair.front.ds_size = index + 1;
    pair.front.ds_string[index] = 0;

    return pair;
}


DynString ds_slice(DynString* input, size_t start, size_t end) {
    assert(end < input->ds_size);
    assert(start < end);

    // Mark end and copy
    input->ds_string[end] = 0;
    DynString output = ds_from_string(input->ds_string + start);

    // Cleanup
    ds_free(input);
    return output;
}


DynString ds_clear(DynString* input) {
    DynString output = ds__move(input);
    output.ds_size = 0;
    return output;
}


DynString ds_push(DynString* input, char chr) {
    return ds_insert(input, input->ds_size, chr);
}


DynString ds_pop(DynString* input, char* out) {
    return ds_remove(input, input->ds_size - 1, out);
}


DynString ds_insert(DynString* input, size_t index, char chr) {
    assert(index <= input->ds_size);

    DynString output = ds__move(input);

    // If full, double capacity
    if (output.ds_size == output.ds_capacity) {
        output = ds__grow(&output);
    }

    // Grow
    output.ds_size++;

    // Shift characters
    for (size_t idx = output.ds_size; index != idx; --idx) {
        output.ds_string[idx] = output.ds_string[idx - 1];
    }

    // Insert
    output.ds_string[index] = chr;
    return output;
}


DynString ds_remove(DynString* input, size_t index, char* out) {
    assert(index < input->ds_size);
    assert(input->ds_size > 1);

    // Get character
    if (out) { // Check NULL
        *out = input->ds_string[index];
    }

    // Shift tail
    for (size_t idx = index; idx != input->ds_size - 1; ++idx) {
        input->ds_string[idx] = input->ds_string[idx + 1];
    }

    // Truncate
    input->ds_size--;
    return ds__move(input);
}


char* ds_to_string(DynString* out) {
    char* cstr = out->ds_string;
    ds__reset(out);

    return cstr;
}


void ds_free(DynString* out) {
    free(out->ds_string);
    ds__reset(out);
}


char ds_index(DynString input, size_t index) {
    assert(index < input.ds_size);
    return input.ds_string[index];
}


char const* ds_string(DynString input) {
    return input.ds_string;
}


size_t ds_size(DynString input) {
    return input.ds_size;
}


size_t ds_capacity(DynString input) {
    return input.ds_capacity;
}
