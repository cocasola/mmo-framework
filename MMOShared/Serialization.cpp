//
// Created by Keegan Beaulieu on 2024-07-10.
//

#include "Serialization.h"



//
// #include <iostream>
//
// List<Serializable> Serialization::serializables;
// std::unordered_map<int, Serializable *> Serialization::serializable_map;
//
// struct VectorInfo
// {
//     size_t elements;
//     unsigned char *bytes;
// };
//
// void Serialization::register_serializable(const Serializable &_serializable)
// {
//     Serializable *serializable = serializables.alloc(_serializable);
//     serializable_map[_serializable.id] = serializable;
// }
//
// static VectorInfo start_serialize_vector(unsigned char *buf, size_t buf_size, unsigned char *structure, size_t offset, size_t element_size)
// {
//     std::vector<unsigned char> *vector = (std::vector<unsigned char> *)(structure + offset);
//     size_t elements = vector->size()/element_size;
//     unsigned char *data = vector->data();
//
//     buf[0] = elements;
//
//     return {
//         elements,
//         data
//     };
// }
//
// size_t Serialization::serialize(int id, void *_structure, unsigned char *buf, size_t buf_size)
// {
//     Serializable *serializable = serializable_map[id];
//     unsigned char *structure = (unsigned char *)_structure;
//     size_t cursor = 0;
//
//     for (auto i : serializable->members) {
//         switch (i.type) {
//             case Serializable::Member::PRIMITIVE:
//                 for (int j = 0; j < i.size; ++j) {
//                     buf[cursor++] = *(structure + i.offset + j);
//                 }
//                 break;
//             case Serializable::Member::SERIALIZABLE: {
//                     size_t bytes = serialize(i.id, structure + i.offset, buf + cursor, buf_size - cursor);
//                     cursor += bytes;
//                 } break;
//             case Serializable::Member::PRIMITIVE_VECTOR: {
//                     VectorInfo info = start_serialize_vector(buf + cursor, buf_size - cursor, structure, i.offset, i.size);
//                     cursor += 1;
//
//                     for (int j = 0; j < info.elements; ++j) {
//                         for (int k = 0; k < i.size; ++k) {
//                             buf[cursor++] = *(info.bytes + j*i.size + k);
//                         }
//                     }
//                 } break;
//             case Serializable::Member::SERIALIZABLE_VECTOR: {
//                     Serializable *serializable = serializable_map[i.id];
//                     VectorInfo info = start_serialize_vector(buf + cursor, buf_size - cursor, structure, i.offset, serializable->size);
//                     cursor += 1;
//
//                     for (int j = 0; j < info.elements; ++j) {
//                         size_t bytes = serialize(i.id, info.bytes + j*serializable->size, buf + cursor, buf_size - cursor);
//                         cursor += bytes;
//                     }
//                 } break;
//         }
//     }
//
//     return cursor;
// }
//
// size_t Serialization::deserialize(int id, void *_structure, unsigned char *buf, size_t buf_size)
// {
//     Serializable *serializable = serializable_map[id];
//     unsigned char *structure = (unsigned char *)_structure;
//
//     int cursor = 0;
//
//     for (auto i : serializable->members) {
//         switch (i.type) {
//             case Serializable::Member::PRIMITIVE:
//                 for (int j = 0; j < i.size; ++j) {
//                     structure[i.offset + j] = buf[cursor++];
//                 }
//                 break;
//             case Serializable::Member::SERIALIZABLE:
//                 cursor += deserialize(i.id, structure + i.offset, buf + cursor, buf_size - cursor);
//                 break;
//             case Serializable::Member::PRIMITIVE_VECTOR: {
//                     unsigned char elements = buf[cursor++];
//
//                     std::vector<unsigned char> vector;
//                     vector.resize(elements*i.size);
//
//                     memcpy(vector.data(), buf + cursor, vector.size());
//                     cursor += vector.size();
//
//                     *(std::vector<unsigned char> *)(structure + i.offset) = vector;
//                 } break;
//             case Serializable::Member::SERIALIZABLE_VECTOR: {
//                     Serializable *member_serializable = serializable_map[i.id];
//
//                     unsigned char elements = buf[cursor++];
//
//                     std::vector<unsigned char> vector;
//                     vector.resize(elements*member_serializable->size);
//
//                     for (int j = 0; j < elements; ++j) {
//                         cursor += deserialize(i.id, vector.data() + j*member_serializable->size, buf + cursor, buf_size - cursor);
//                     }
//
//                     *(std::vector<unsigned char> *)(structure + i.offset) = vector;
//                 } break;
//         }
//     }
//
//     return cursor;
// }
