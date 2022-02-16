//
// Created by mbart on 16/02/2022.
//

#ifndef LAB_SHELL_DYNSTRING_H
#define LAB_SHELL_DYNSTRING_H

typedef struct DynString DynString;
struct DynString {
    char*   ds_string;
    size_t  ds_size;
    size_t  ds_capacity;
};

/// Pair of DynStrings, has a .front and .tail.
typedef struct DynStringPair DynStringPair;
struct DynStringPair {
    DynString front;
    DynString tail;
};

/// Makes a new empty DynString.
DynString ds_new();

/// Creates a new empty, pre-allocated DynString with the given capacity.
DynString ds_with_capacity(size_t ds_capacity);

/// Creates a new DynString from the provided C-string.
DynString ds_from_string(char const ds_string[static 1]);

/// Creates a new DynString by copying an existing DynString.
DynString ds_copy_ds(DynString ds_copy);

/// Concatenates two DynStrings, the originals remain available.
DynString ds_concat(DynString ds_front, DynString ds_tail);

/// Concatenates and consumes two DynStrings.
DynString ds_combine(DynString* ds_front, DynString* ds_tail);

/// Inserts a DynString at the given index of another DynString. Consumes its input.
DynString ds_splice(DynString* ds_source, DynString* ds_insert, size_t ds_index);

/// Splits a DynString into two pieces, returning a pair struct. Consumes its input.
DynStringPair ds_split(DynString* ds_input, size_t ds_index);

/// Takes a substring of a DynString. Consumes its input.
DynString ds_slice(DynString* ds_input, size_t ds_start, size_t ds_end);

/// Clears the contents of the DynString without de-allocating. Consumes its input.
DynString ds_clear(DynString* ds_input);

/// Pushes a character at the end of the DynString. Consumes its input.
DynString ds_push(DynString* ds_input, char ds_char);

/// Pops the last character of the DynString, saving it at the provided address, if any. Consumes its input.
DynString ds_pop(DynString* ds_input, char* ds_out);

/// Inserts a character at the given index. Consumes its input.
DynString ds_insert(DynString* ds_input, size_t ds_index, char ds_char);

/// Removes a character at the given index, saving it at the provided address, if any. Consumes its input.
DynString ds_remove(DynString* ds_input, size_t ds_index, char* ds_out);

/// Consumes a DynString and returns a dynamically allocated C-string that must be freed.
char* ds_to_string(DynString* ds_out);

/// De-allocates and clears a DynString
void ds_free(DynString* ds_out);

/// Returns the character at the specified index
char ds_index(DynString ds_input, size_t ds_index);

/// Returns an immutable view of the underlying string
char const* ds_string(DynString ds_input);

/// Returns the size of the string
size_t ds_size(DynString ds_input);

/// Returns the memory capacity of the DynString
size_t ds_capacity(DynString ds_input);

#endif //LAB_SHELL_DYNSTRING_H
