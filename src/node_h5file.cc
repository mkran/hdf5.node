#include <node.h>
#include <string>

#include "H5Cpp.h"
#include "node_h5file.h"

namespace NodeHDF5 {
    
    using namespace v8;
    using namespace H5;

    File::File (std::string path) {
    
        m_file = new H5File(path.c_str(), H5F_ACC_RDONLY);
        
    }
    
    File::~File () {
        
        delete m_file;
        
    }
    
    void File::Initialize (Handle<Object> target) {
        
        HandleScope scope;
        
        // instantiate constructor template
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->InstanceTemplate()->SetInternalFieldCount(1);
        
        // member method prototypes
        SetPrototypeMethod(t, "close", Close);
        
        // specify constructor function
        target->Set(String::NewSymbol("H5File"), t->GetFunction());
        
    }
    
    Handle<Value> File::New (const Arguments& args) {
        
        HandleScope scope;
        
        if (args.Length() != 1 || !args[0]->IsString()) {
            
            // fail out if arguments are not correct
            ThrowException(v8::Exception::SyntaxError(String::New("expected file path")));
            return scope.Close(Undefined());
            
        }
        
        // store specified file path
        String::Utf8Value path (args[0]->ToString());
        
        // also store cstring version
        std::string c_path (*path);
        
        if (!H5::H5File::isHdf5(c_path.c_str())) {
            
            // fail out if file is not valid hdf5
            ThrowException(v8::Exception::TypeError(String::New("file is not hdf5 format")));
            return scope.Close(Undefined());
            
        }
        
        // instantiate h5 file object
        File* f = new File(c_path);
        f->Wrap(args.This());
        
        // attach various properties
        args.This()->Set(String::NewSymbol("path"), String::New(f->m_file->getFileName().c_str()));
        args.This()->Set(String::NewSymbol("size"), Number::New(f->m_file->getFileSize()));
        
        return args.This();
        
    }
    
    Handle<Value> File::Close (const Arguments& args) {
        
        HandleScope scope;
        
        File* f = ObjectWrap::Unwrap<File>(args.This());
        f->m_file->close();
        
        return scope.Close(args.This());
        
    }

};