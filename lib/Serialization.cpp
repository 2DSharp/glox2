//
// Created by dedip on 19-02-2023.
//

#include "../include/Serialization.h"

#include "../serialization/schema_generated.h"

void Serialization::save_state(VM *vm, const char * location) {
    flatbuffers::FlatBufferBuilder builder(1024);
    auto cmd = glox::G_Op(ICONST);
    auto val = glox::G_N(87);


    auto b1 = glox::CreateG_Bytecode(builder, glox::DataType_OP, glox::DataVal_op, builder.CreateStruct(cmd).Union());
    auto b2 = glox::CreateG_Bytecode(builder, glox::DataType_INT, glox::DataVal_n, builder.CreateStruct(val).Union());
    auto bvec = builder.CreateVector({b1, b2, glox::CreateG_Bytecode(builder, glox::DataType_INT, glox::DataVal_n, builder.CreateStruct(val).Union()), glox::CreateG_Bytecode(builder, glox::DataType_INT, glox::DataVal_n, builder.CreateStruct(val).Union()),glox::CreateG_Bytecode(builder, glox::DataType_INT, glox::DataVal_n, builder.CreateStruct(val).Union())});

    auto exec = glox::Createg_executable(builder, bvec);
    builder.Finish(exec);

    uint8_t *buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();


// Open a file for writing
    FILE *f = fopen(location, "wb");

// Write the serialized data to the file
    fwrite(buf, 1, size, f);

// Close the file
    fclose(f);

}
