#pragma once
#include <memory>
#include <string>
#include <utils.h>
namespace rst
{
    struct Object;
    using ObjPtr = std::shared_ptr<Object>;
    struct Object
    {
    public:
        virtual ~Object() {}
        virtual std::string to_string() const { return "Object"; }
        virtual ObjPtr add(Object *rhs)
        {
            rst_error(RUNTIME_ERROR, "object not implement add operation!");
            return nullptr;
        }
        virtual ObjPtr sub(Object *rhs)
        {
            rst_error(RUNTIME_ERROR, "object not implement sub operation!");
            return nullptr;
        }
        virtual ObjPtr mul(Object *rhs)
        {
            rst_error(RUNTIME_ERROR, "Object not implement mul operation!");
            return nullptr;
        };

        virtual ObjPtr div(Object *rhs)
        {
            rst_error(RUNTIME_ERROR, "Object not implement div operation!");
            return nullptr;
        };

    private:
    };
    struct Integer : public Object
    {
    public:
        Integer(int64_t _val) : val(_val) {}
        std::string to_string() const override { return std::to_string(val); }
        static int64_t get_val(Object *obj)
        {
            return static_cast<Integer *>(obj)->val;
        }
        ObjPtr add(Object *rhs) override
        {
            return std::make_shared<Integer>(val + get_val(rhs));
        }
        ObjPtr sub(Object *rhs) override
        {
            return std::make_shared<Integer>(val - get_val(rhs));
        }
        ObjPtr mul(Object *rhs) override
        {
            return std::make_shared<Integer>(val * get_val(rhs));
        }
        ObjPtr div(Object *rhs) override
        {
            return std::make_shared<Integer>(val / get_val(rhs));
        }

    private:
        int64_t val;
    };
}