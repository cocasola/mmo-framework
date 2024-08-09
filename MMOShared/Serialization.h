//
// Created by Keegan Beaulieu on 2024-07-10.
//

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stddef.h>
#include <vector>

struct SerializableMember
{
    enum Type
    {
        PRIMITIVE,
        PRIMITIVE_VECTOR
    };

    Type type;
    size_t offset;
    size_t size;
};

typedef void(*Deserializer)(unsigned char *&bytes, unsigned char *end, const void *structure);
typedef void(*Serializer)(unsigned char *&bytes, unsigned char *end, void *structure);

#define PRIMITIVE(structure, member) (SerializableMember){ SerializableMember::Type::PRIMITIVE, offsetof(structure, member), sizeof(structure().member) },
#define VECTOR_PRIMITIVE(structure, member) (SerializableMember){ SerializableMember::Type::PRIMITIVE_VECTOR, offsetof(structure, member), sizeof(*member.data()) },

#define SERIALIZABLE(members) \
    static inline void serialize(unsigned char *&bytes, unsigned char *end, const void *structure) \
    { \
        std::vector<SerializableMember> __members = members; \
        unsigned char *ptr = (unsigned char *)structure; \
        \
        for (auto i : __members) { \
            switch (i.type) { \
                case SerializableMember::Type::PRIMITIVE: \
                    for (int j = 0; j < i.size; ++j) { \
                        *(bytes++) = ptr[i.offset + j]; \
                    } \
                    break; \
                case SerializableMember::Type::PRIMITIVE_VECTOR: { \
                    auto vec = (std::vector<unsigned char> *)(ptr + i.offset); \
                    *(bytes++) = vec->size()/i.size; \
                    for (int j = 0; j < vec->size(); ++j) { \
                        *(bytes++) = vec->data()[j]; \
                    } \
                    \
                    } break; \
            } \
        } \
    } \
    \
    static inline void deserialize(unsigned char *&bytes, unsigned char *end, void *structure) \
    { \
        std::vector<SerializableMember> __members = members; \
        unsigned char *ptr = (unsigned char *)structure; \
        \
        for (auto i : __members) { \
            switch (i.type) { \
                case SerializableMember::Type::PRIMITIVE: \
                    for (int j = 0; j < i.size; ++j) { \
                        ptr[i.offset + j] = *(bytes++); \
                    } \
                    break; \
                case SerializableMember::Type::PRIMITIVE_VECTOR: \
                    auto vec = (std::vector<unsigned char> *)(ptr + i.offset); \
                    *vec = std::vector<unsigned char>(*(bytes++)*i.size, 0); \
                    memcpy(vec->data(), bytes, vec->size()); \
                    bytes += vec->size(); \
                    break; \
            } \
        } \
    } \

template <typename T>
static inline void serialize_primitive(unsigned char *&bytes, unsigned char *end, const T &data)
{
    memcpy(bytes, &data, sizeof(T));
    bytes += sizeof(T);
}

template <typename T>
static inline void deserialize_primitive(unsigned char *&bytes, unsigned char *end, T &data)
{
    memset(&data, 0, sizeof(T));
    memcpy(&data, bytes, sizeof(T));
    bytes += sizeof(T);
}

#endif //SERIALIZATION_H
