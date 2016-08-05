// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: analyzer.proto

#ifndef PROTOBUF_analyzer_2eproto__INCLUDED
#define PROTOBUF_analyzer_2eproto__INCLUDED

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4244 4267)
#endif

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace analyzer {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_analyzer_2eproto();
void protobuf_AssignDesc_analyzer_2eproto();
void protobuf_ShutdownFile_analyzer_2eproto();

class Layer;
class Info;
class RecordTuple;
class Recorder;

// ===================================================================

class Layer : public ::google::protobuf::Message {
 public:
  Layer();
  virtual ~Layer();
  
  Layer(const Layer& from);
  
  inline Layer& operator=(const Layer& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Layer& default_instance();
  
  void Swap(Layer* other);
  
  // implements Message ----------------------------------------------
  
  Layer* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Layer& from);
  void MergeFrom(const Layer& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 count = 1;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 1;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);
  
  // optional string type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline const ::std::string& type() const;
  inline void set_type(const ::std::string& value);
  inline void set_type(const char* value);
  inline void set_type(const char* value, size_t size);
  inline ::std::string* mutable_type();
  inline ::std::string* release_type();
  
  // optional int32 num = 3;
  inline bool has_num() const;
  inline void clear_num();
  static const int kNumFieldNumber = 3;
  inline ::google::protobuf::int32 num() const;
  inline void set_num(::google::protobuf::int32 value);
  
  // optional int32 channels = 4;
  inline bool has_channels() const;
  inline void clear_channels();
  static const int kChannelsFieldNumber = 4;
  inline ::google::protobuf::int32 channels() const;
  inline void set_channels(::google::protobuf::int32 value);
  
  // optional int32 height = 5;
  inline bool has_height() const;
  inline void clear_height();
  static const int kHeightFieldNumber = 5;
  inline ::google::protobuf::int32 height() const;
  inline void set_height(::google::protobuf::int32 value);
  
  // optional int32 width = 6;
  inline bool has_width() const;
  inline void clear_width();
  static const int kWidthFieldNumber = 6;
  inline ::google::protobuf::int32 width() const;
  inline void set_width(::google::protobuf::int32 value);
  
  // optional string name = 7;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 7;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // repeated float weight = 8;
  inline int weight_size() const;
  inline void clear_weight();
  static const int kWeightFieldNumber = 8;
  inline float weight(int index) const;
  inline void set_weight(int index, float value);
  inline void add_weight(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      weight() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_weight();
  
  // repeated float grad = 9;
  inline int grad_size() const;
  inline void clear_grad();
  static const int kGradFieldNumber = 9;
  inline float grad(int index) const;
  inline void set_grad(int index, float value);
  inline void add_grad(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      grad() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_grad();
  
  // repeated float stat = 10;
  inline int stat_size() const;
  inline void clear_stat();
  static const int kStatFieldNumber = 10;
  inline float stat(int index) const;
  inline void set_stat(int index, float value);
  inline void add_stat(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      stat() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_stat();
  
  // repeated float distance = 11;
  inline int distance_size() const;
  inline void clear_distance();
  static const int kDistanceFieldNumber = 11;
  inline float distance(int index) const;
  inline void set_distance(int index, float value);
  inline void add_distance(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      distance() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_distance();
  
  // @@protoc_insertion_point(class_scope:analyzer.Layer)
 private:
  inline void set_has_count();
  inline void clear_has_count();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_num();
  inline void clear_has_num();
  inline void set_has_channels();
  inline void clear_has_channels();
  inline void set_has_height();
  inline void clear_has_height();
  inline void set_has_width();
  inline void clear_has_width();
  inline void set_has_name();
  inline void clear_has_name();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* type_;
  ::google::protobuf::int32 count_;
  ::google::protobuf::int32 num_;
  ::google::protobuf::int32 channels_;
  ::google::protobuf::int32 height_;
  ::std::string* name_;
  ::google::protobuf::RepeatedField< float > weight_;
  ::google::protobuf::RepeatedField< float > grad_;
  ::google::protobuf::RepeatedField< float > stat_;
  ::google::protobuf::RepeatedField< float > distance_;
  ::google::protobuf::int32 width_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(11 + 31) / 32];
  
  friend void  protobuf_AddDesc_analyzer_2eproto();
  friend void protobuf_AssignDesc_analyzer_2eproto();
  friend void protobuf_ShutdownFile_analyzer_2eproto();
  
  void InitAsDefaultInstance();
  static Layer* default_instance_;
};
// -------------------------------------------------------------------

class Info : public ::google::protobuf::Message {
 public:
  Info();
  virtual ~Info();
  
  Info(const Info& from);
  
  inline Info& operator=(const Info& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Info& default_instance();
  
  void Swap(Info* other);
  
  // implements Message ----------------------------------------------
  
  Info* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Info& from);
  void MergeFrom(const Info& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string filename = 1;
  inline bool has_filename() const;
  inline void clear_filename();
  static const int kFilenameFieldNumber = 1;
  inline const ::std::string& filename() const;
  inline void set_filename(const ::std::string& value);
  inline void set_filename(const char* value);
  inline void set_filename(const char* value, size_t size);
  inline ::std::string* mutable_filename();
  inline ::std::string* release_filename();
  
  // optional int32 iteration = 2;
  inline bool has_iteration() const;
  inline void clear_iteration();
  static const int kIterationFieldNumber = 2;
  inline ::google::protobuf::int32 iteration() const;
  inline void set_iteration(::google::protobuf::int32 value);
  
  // optional int32 worker_id = 3;
  inline bool has_worker_id() const;
  inline void clear_worker_id();
  static const int kWorkerIdFieldNumber = 3;
  inline ::google::protobuf::int32 worker_id() const;
  inline void set_worker_id(::google::protobuf::int32 value);
  
  // optional int32 sim_id = 4;
  inline bool has_sim_id() const;
  inline void clear_sim_id();
  static const int kSimIdFieldNumber = 4;
  inline ::google::protobuf::int32 sim_id() const;
  inline void set_sim_id(::google::protobuf::int32 value);
  
  // repeated .analyzer.Layer layers = 5;
  inline int layers_size() const;
  inline void clear_layers();
  static const int kLayersFieldNumber = 5;
  inline const ::analyzer::Layer& layers(int index) const;
  inline ::analyzer::Layer* mutable_layers(int index);
  inline ::analyzer::Layer* add_layers();
  inline const ::google::protobuf::RepeatedPtrField< ::analyzer::Layer >&
      layers() const;
  inline ::google::protobuf::RepeatedPtrField< ::analyzer::Layer >*
      mutable_layers();
  
  // @@protoc_insertion_point(class_scope:analyzer.Info)
 private:
  inline void set_has_filename();
  inline void clear_has_filename();
  inline void set_has_iteration();
  inline void clear_has_iteration();
  inline void set_has_worker_id();
  inline void clear_has_worker_id();
  inline void set_has_sim_id();
  inline void clear_has_sim_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* filename_;
  ::google::protobuf::int32 iteration_;
  ::google::protobuf::int32 worker_id_;
  ::google::protobuf::RepeatedPtrField< ::analyzer::Layer > layers_;
  ::google::protobuf::int32 sim_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void  protobuf_AddDesc_analyzer_2eproto();
  friend void protobuf_AssignDesc_analyzer_2eproto();
  friend void protobuf_ShutdownFile_analyzer_2eproto();
  
  void InitAsDefaultInstance();
  static Info* default_instance_;
};
// -------------------------------------------------------------------

class RecordTuple : public ::google::protobuf::Message {
 public:
  RecordTuple();
  virtual ~RecordTuple();
  
  RecordTuple(const RecordTuple& from);
  
  inline RecordTuple& operator=(const RecordTuple& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const RecordTuple& default_instance();
  
  void Swap(RecordTuple* other);
  
  // implements Message ----------------------------------------------
  
  RecordTuple* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RecordTuple& from);
  void MergeFrom(const RecordTuple& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 iteration = 1;
  inline bool has_iteration() const;
  inline void clear_iteration();
  static const int kIterationFieldNumber = 1;
  inline ::google::protobuf::int32 iteration() const;
  inline void set_iteration(::google::protobuf::int32 value);
  
  // optional float value = 2;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 2;
  inline float value() const;
  inline void set_value(float value);
  
  // optional string type = 3;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 3;
  inline const ::std::string& type() const;
  inline void set_type(const ::std::string& value);
  inline void set_type(const char* value);
  inline void set_type(const char* value, size_t size);
  inline ::std::string* mutable_type();
  inline ::std::string* release_type();
  
  // repeated float data = 4;
  inline int data_size() const;
  inline void clear_data();
  static const int kDataFieldNumber = 4;
  inline float data(int index) const;
  inline void set_data(int index, float value);
  inline void add_data(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      data() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_data();
  
  // @@protoc_insertion_point(class_scope:analyzer.RecordTuple)
 private:
  inline void set_has_iteration();
  inline void clear_has_iteration();
  inline void set_has_value();
  inline void clear_has_value();
  inline void set_has_type();
  inline void clear_has_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 iteration_;
  float value_;
  ::std::string* type_;
  ::google::protobuf::RepeatedField< float > data_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_analyzer_2eproto();
  friend void protobuf_AssignDesc_analyzer_2eproto();
  friend void protobuf_ShutdownFile_analyzer_2eproto();
  
  void InitAsDefaultInstance();
  static RecordTuple* default_instance_;
};
// -------------------------------------------------------------------

class Recorder : public ::google::protobuf::Message {
 public:
  Recorder();
  virtual ~Recorder();
  
  Recorder(const Recorder& from);
  
  inline Recorder& operator=(const Recorder& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const Recorder& default_instance();
  
  void Swap(Recorder* other);
  
  // implements Message ----------------------------------------------
  
  Recorder* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Recorder& from);
  void MergeFrom(const Recorder& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // repeated .analyzer.RecordTuple tuple = 2;
  inline int tuple_size() const;
  inline void clear_tuple();
  static const int kTupleFieldNumber = 2;
  inline const ::analyzer::RecordTuple& tuple(int index) const;
  inline ::analyzer::RecordTuple* mutable_tuple(int index);
  inline ::analyzer::RecordTuple* add_tuple();
  inline const ::google::protobuf::RepeatedPtrField< ::analyzer::RecordTuple >&
      tuple() const;
  inline ::google::protobuf::RepeatedPtrField< ::analyzer::RecordTuple >*
      mutable_tuple();
  
  // @@protoc_insertion_point(class_scope:analyzer.Recorder)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* name_;
  ::google::protobuf::RepeatedPtrField< ::analyzer::RecordTuple > tuple_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_analyzer_2eproto();
  friend void protobuf_AssignDesc_analyzer_2eproto();
  friend void protobuf_ShutdownFile_analyzer_2eproto();
  
  void InitAsDefaultInstance();
  static Recorder* default_instance_;
};
// ===================================================================


// ===================================================================

// Layer

// optional int32 count = 1;
inline bool Layer::has_count() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Layer::set_has_count() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Layer::clear_has_count() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Layer::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 Layer::count() const {
  return count_;
}
inline void Layer::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// optional string type = 2;
inline bool Layer::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Layer::set_has_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Layer::clear_has_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Layer::clear_type() {
  if (type_ != &::google::protobuf::internal::kEmptyString) {
    type_->clear();
  }
  clear_has_type();
}
inline const ::std::string& Layer::type() const {
  return *type_;
}
inline void Layer::set_type(const ::std::string& value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void Layer::set_type(const char* value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void Layer::set_type(const char* value, size_t size) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Layer::mutable_type() {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  return type_;
}
inline ::std::string* Layer::release_type() {
  clear_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = type_;
    type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 num = 3;
inline bool Layer::has_num() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Layer::set_has_num() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Layer::clear_has_num() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Layer::clear_num() {
  num_ = 0;
  clear_has_num();
}
inline ::google::protobuf::int32 Layer::num() const {
  return num_;
}
inline void Layer::set_num(::google::protobuf::int32 value) {
  set_has_num();
  num_ = value;
}

// optional int32 channels = 4;
inline bool Layer::has_channels() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Layer::set_has_channels() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Layer::clear_has_channels() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Layer::clear_channels() {
  channels_ = 0;
  clear_has_channels();
}
inline ::google::protobuf::int32 Layer::channels() const {
  return channels_;
}
inline void Layer::set_channels(::google::protobuf::int32 value) {
  set_has_channels();
  channels_ = value;
}

// optional int32 height = 5;
inline bool Layer::has_height() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Layer::set_has_height() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Layer::clear_has_height() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Layer::clear_height() {
  height_ = 0;
  clear_has_height();
}
inline ::google::protobuf::int32 Layer::height() const {
  return height_;
}
inline void Layer::set_height(::google::protobuf::int32 value) {
  set_has_height();
  height_ = value;
}

// optional int32 width = 6;
inline bool Layer::has_width() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Layer::set_has_width() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Layer::clear_has_width() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Layer::clear_width() {
  width_ = 0;
  clear_has_width();
}
inline ::google::protobuf::int32 Layer::width() const {
  return width_;
}
inline void Layer::set_width(::google::protobuf::int32 value) {
  set_has_width();
  width_ = value;
}

// optional string name = 7;
inline bool Layer::has_name() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Layer::set_has_name() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Layer::clear_has_name() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Layer::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& Layer::name() const {
  return *name_;
}
inline void Layer::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void Layer::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void Layer::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Layer::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* Layer::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated float weight = 8;
inline int Layer::weight_size() const {
  return weight_.size();
}
inline void Layer::clear_weight() {
  weight_.Clear();
}
inline float Layer::weight(int index) const {
  return weight_.Get(index);
}
inline void Layer::set_weight(int index, float value) {
  weight_.Set(index, value);
}
inline void Layer::add_weight(float value) {
  weight_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
Layer::weight() const {
  return weight_;
}
inline ::google::protobuf::RepeatedField< float >*
Layer::mutable_weight() {
  return &weight_;
}

// repeated float grad = 9;
inline int Layer::grad_size() const {
  return grad_.size();
}
inline void Layer::clear_grad() {
  grad_.Clear();
}
inline float Layer::grad(int index) const {
  return grad_.Get(index);
}
inline void Layer::set_grad(int index, float value) {
  grad_.Set(index, value);
}
inline void Layer::add_grad(float value) {
  grad_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
Layer::grad() const {
  return grad_;
}
inline ::google::protobuf::RepeatedField< float >*
Layer::mutable_grad() {
  return &grad_;
}

// repeated float stat = 10;
inline int Layer::stat_size() const {
  return stat_.size();
}
inline void Layer::clear_stat() {
  stat_.Clear();
}
inline float Layer::stat(int index) const {
  return stat_.Get(index);
}
inline void Layer::set_stat(int index, float value) {
  stat_.Set(index, value);
}
inline void Layer::add_stat(float value) {
  stat_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
Layer::stat() const {
  return stat_;
}
inline ::google::protobuf::RepeatedField< float >*
Layer::mutable_stat() {
  return &stat_;
}

// repeated float distance = 11;
inline int Layer::distance_size() const {
  return distance_.size();
}
inline void Layer::clear_distance() {
  distance_.Clear();
}
inline float Layer::distance(int index) const {
  return distance_.Get(index);
}
inline void Layer::set_distance(int index, float value) {
  distance_.Set(index, value);
}
inline void Layer::add_distance(float value) {
  distance_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
Layer::distance() const {
  return distance_;
}
inline ::google::protobuf::RepeatedField< float >*
Layer::mutable_distance() {
  return &distance_;
}

// -------------------------------------------------------------------

// Info

// optional string filename = 1;
inline bool Info::has_filename() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Info::set_has_filename() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Info::clear_has_filename() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Info::clear_filename() {
  if (filename_ != &::google::protobuf::internal::kEmptyString) {
    filename_->clear();
  }
  clear_has_filename();
}
inline const ::std::string& Info::filename() const {
  return *filename_;
}
inline void Info::set_filename(const ::std::string& value) {
  set_has_filename();
  if (filename_ == &::google::protobuf::internal::kEmptyString) {
    filename_ = new ::std::string;
  }
  filename_->assign(value);
}
inline void Info::set_filename(const char* value) {
  set_has_filename();
  if (filename_ == &::google::protobuf::internal::kEmptyString) {
    filename_ = new ::std::string;
  }
  filename_->assign(value);
}
inline void Info::set_filename(const char* value, size_t size) {
  set_has_filename();
  if (filename_ == &::google::protobuf::internal::kEmptyString) {
    filename_ = new ::std::string;
  }
  filename_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Info::mutable_filename() {
  set_has_filename();
  if (filename_ == &::google::protobuf::internal::kEmptyString) {
    filename_ = new ::std::string;
  }
  return filename_;
}
inline ::std::string* Info::release_filename() {
  clear_has_filename();
  if (filename_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = filename_;
    filename_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 iteration = 2;
inline bool Info::has_iteration() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Info::set_has_iteration() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Info::clear_has_iteration() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Info::clear_iteration() {
  iteration_ = 0;
  clear_has_iteration();
}
inline ::google::protobuf::int32 Info::iteration() const {
  return iteration_;
}
inline void Info::set_iteration(::google::protobuf::int32 value) {
  set_has_iteration();
  iteration_ = value;
}

// optional int32 worker_id = 3;
inline bool Info::has_worker_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Info::set_has_worker_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Info::clear_has_worker_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Info::clear_worker_id() {
  worker_id_ = 0;
  clear_has_worker_id();
}
inline ::google::protobuf::int32 Info::worker_id() const {
  return worker_id_;
}
inline void Info::set_worker_id(::google::protobuf::int32 value) {
  set_has_worker_id();
  worker_id_ = value;
}

// optional int32 sim_id = 4;
inline bool Info::has_sim_id() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Info::set_has_sim_id() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Info::clear_has_sim_id() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Info::clear_sim_id() {
  sim_id_ = 0;
  clear_has_sim_id();
}
inline ::google::protobuf::int32 Info::sim_id() const {
  return sim_id_;
}
inline void Info::set_sim_id(::google::protobuf::int32 value) {
  set_has_sim_id();
  sim_id_ = value;
}

// repeated .analyzer.Layer layers = 5;
inline int Info::layers_size() const {
  return layers_.size();
}
inline void Info::clear_layers() {
  layers_.Clear();
}
inline const ::analyzer::Layer& Info::layers(int index) const {
  return layers_.Get(index);
}
inline ::analyzer::Layer* Info::mutable_layers(int index) {
  return layers_.Mutable(index);
}
inline ::analyzer::Layer* Info::add_layers() {
  return layers_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::analyzer::Layer >&
Info::layers() const {
  return layers_;
}
inline ::google::protobuf::RepeatedPtrField< ::analyzer::Layer >*
Info::mutable_layers() {
  return &layers_;
}

// -------------------------------------------------------------------

// RecordTuple

// optional int32 iteration = 1;
inline bool RecordTuple::has_iteration() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RecordTuple::set_has_iteration() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RecordTuple::clear_has_iteration() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RecordTuple::clear_iteration() {
  iteration_ = 0;
  clear_has_iteration();
}
inline ::google::protobuf::int32 RecordTuple::iteration() const {
  return iteration_;
}
inline void RecordTuple::set_iteration(::google::protobuf::int32 value) {
  set_has_iteration();
  iteration_ = value;
}

// optional float value = 2;
inline bool RecordTuple::has_value() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RecordTuple::set_has_value() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RecordTuple::clear_has_value() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RecordTuple::clear_value() {
  value_ = 0;
  clear_has_value();
}
inline float RecordTuple::value() const {
  return value_;
}
inline void RecordTuple::set_value(float value) {
  set_has_value();
  value_ = value;
}

// optional string type = 3;
inline bool RecordTuple::has_type() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RecordTuple::set_has_type() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RecordTuple::clear_has_type() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RecordTuple::clear_type() {
  if (type_ != &::google::protobuf::internal::kEmptyString) {
    type_->clear();
  }
  clear_has_type();
}
inline const ::std::string& RecordTuple::type() const {
  return *type_;
}
inline void RecordTuple::set_type(const ::std::string& value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void RecordTuple::set_type(const char* value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void RecordTuple::set_type(const char* value, size_t size) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RecordTuple::mutable_type() {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  return type_;
}
inline ::std::string* RecordTuple::release_type() {
  clear_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = type_;
    type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated float data = 4;
inline int RecordTuple::data_size() const {
  return data_.size();
}
inline void RecordTuple::clear_data() {
  data_.Clear();
}
inline float RecordTuple::data(int index) const {
  return data_.Get(index);
}
inline void RecordTuple::set_data(int index, float value) {
  data_.Set(index, value);
}
inline void RecordTuple::add_data(float value) {
  data_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
RecordTuple::data() const {
  return data_;
}
inline ::google::protobuf::RepeatedField< float >*
RecordTuple::mutable_data() {
  return &data_;
}

// -------------------------------------------------------------------

// Recorder

// optional string name = 1;
inline bool Recorder::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Recorder::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Recorder::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Recorder::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& Recorder::name() const {
  return *name_;
}
inline void Recorder::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void Recorder::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void Recorder::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Recorder::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* Recorder::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .analyzer.RecordTuple tuple = 2;
inline int Recorder::tuple_size() const {
  return tuple_.size();
}
inline void Recorder::clear_tuple() {
  tuple_.Clear();
}
inline const ::analyzer::RecordTuple& Recorder::tuple(int index) const {
  return tuple_.Get(index);
}
inline ::analyzer::RecordTuple* Recorder::mutable_tuple(int index) {
  return tuple_.Mutable(index);
}
inline ::analyzer::RecordTuple* Recorder::add_tuple() {
  return tuple_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::analyzer::RecordTuple >&
Recorder::tuple() const {
  return tuple_;
}
inline ::google::protobuf::RepeatedPtrField< ::analyzer::RecordTuple >*
Recorder::mutable_tuple() {
  return &tuple_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace analyzer

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#endif  // PROTOBUF_analyzer_2eproto__INCLUDED