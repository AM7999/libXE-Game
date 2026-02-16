#ifndef NBT_HPP
#define NBT_HPP

// ============================================================
//  NBT.hpp  —  Simple binary NBT writer
//  Supports: byte, short, int, long, float, double,
//            string, list, compound
//  Usage:
//      NBT::Compound root;
//      root.set("health", NBT::Int(20));
//      root.set("name",   NBT::String("Steve"));
//      root.save("player.nbt");
// ============================================================

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <variant>

namespace NBT {

// ─────────────────────────────────────────────
//  Tag type IDs  (matches Notchian spec)
// ─────────────────────────────────────────────
enum class TagType : uint8_t {
    End      = 0,
    Byte     = 1,
    Short    = 2,
    Int      = 3,
    Long     = 4,
    Float    = 5,
    Double   = 6,
    String   = 8,
    List     = 9,
    Compound = 10,
};

// ─────────────────────────────────────────────
//  Forward declarations
// ─────────────────────────────────────────────
struct Tag;
using TagPtr = std::shared_ptr<Tag>;

// ─────────────────────────────────────────────
//  Binary write helpers  (big-endian)
// ─────────────────────────────────────────────
namespace detail {
    inline void writeByte(std::vector<uint8_t>& buf, uint8_t v) {
        buf.push_back(v);
    }

    inline void writeShort(std::vector<uint8_t>& buf, int16_t v) {
        buf.push_back((v >> 8) & 0xFF);
        buf.push_back( v       & 0xFF);
    }

    inline void writeInt(std::vector<uint8_t>& buf, int32_t v) {
        buf.push_back((v >> 24) & 0xFF);
        buf.push_back((v >> 16) & 0xFF);
        buf.push_back((v >>  8) & 0xFF);
        buf.push_back( v        & 0xFF);
    }

    inline void writeLong(std::vector<uint8_t>& buf, int64_t v) {
        for (int i = 7; i >= 0; --i)
            buf.push_back((v >> (i * 8)) & 0xFF);
    }

    inline void writeFloat(std::vector<uint8_t>& buf, float v) {
        uint32_t bits;
        std::memcpy(&bits, &v, 4);
        writeInt(buf, static_cast<int32_t>(bits));
    }

    inline void writeDouble(std::vector<uint8_t>& buf, double v) {
        uint64_t bits;
        std::memcpy(&bits, &v, 8);
        writeLong(buf, static_cast<int64_t>(bits));
    }

    inline void writeString(std::vector<uint8_t>& buf, const std::string& s) {
        if (s.size() > 32767)
            throw std::length_error("NBT string exceeds max length of 32767");
        writeShort(buf, static_cast<int16_t>(s.size()));
        for (char c : s) buf.push_back(static_cast<uint8_t>(c));
    }
} // namespace detail

// ─────────────────────────────────────────────
//  Base Tag
// ─────────────────────────────────────────────
struct Tag {
    virtual ~Tag() = default;
    virtual TagType type() const = 0;
    virtual void write(std::vector<uint8_t>& buf) const = 0;
    // Write payload only (used inside List)
    virtual void writePayload(std::vector<uint8_t>& buf) const = 0;
};

// ─────────────────────────────────────────────
//  Primitive tags
// ─────────────────────────────────────────────
struct Byte : Tag {
    int8_t value;
    explicit Byte(int8_t v = 0) : value(v) {}
    TagType type() const override { return TagType::Byte; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeByte(buf, static_cast<uint8_t>(value));
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct Short : Tag {
    int16_t value;
    explicit Short(int16_t v = 0) : value(v) {}
    TagType type() const override { return TagType::Short; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeShort(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct Int : Tag {
    int32_t value;
    explicit Int(int32_t v = 0) : value(v) {}
    TagType type() const override { return TagType::Int; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeInt(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct Long : Tag {
    int64_t value;
    explicit Long(int64_t v = 0) : value(v) {}
    TagType type() const override { return TagType::Long; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeLong(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct Float : Tag {
    float value;
    explicit Float(float v = 0.f) : value(v) {}
    TagType type() const override { return TagType::Float; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeFloat(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct Double : Tag {
    double value;
    explicit Double(double v = 0.0) : value(v) {}
    TagType type() const override { return TagType::Double; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeDouble(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

struct String : Tag {
    std::string value;
    explicit String(std::string v = "") : value(std::move(v)) {}
    TagType type() const override { return TagType::String; }
    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeString(buf, value);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

// ─────────────────────────────────────────────
//  List tag  — all children must share one type
// ─────────────────────────────────────────────
struct List : Tag {
    TagType elementType;
    std::vector<TagPtr> children;

    // Construct an empty typed list
    explicit List(TagType elemType = TagType::End) : elementType(elemType) {}

    void add(TagPtr tag) {
        if (tag->type() != elementType)
            throw std::invalid_argument("NBT::List — all elements must share the same tag type");
        children.push_back(std::move(tag));
    }

    // Convenience helpers for common types
    void addByte  (int8_t  v) { add(std::make_shared<Byte>  (v)); }
    void addShort (int16_t v) { add(std::make_shared<Short> (v)); }
    void addInt   (int32_t v) { add(std::make_shared<Int>   (v)); }
    void addLong  (int64_t v) { add(std::make_shared<Long>  (v)); }
    void addFloat (float   v) { add(std::make_shared<Float> (v)); }
    void addDouble(double  v) { add(std::make_shared<Double>(v)); }
    void addString(const std::string& v) { add(std::make_shared<String>(v)); }

    TagType type() const override { return TagType::List; }

    void writePayload(std::vector<uint8_t>& buf) const override {
        detail::writeByte(buf, static_cast<uint8_t>(elementType));
        detail::writeInt (buf, static_cast<int32_t>(children.size()));
        for (const auto& child : children)
            child->writePayload(buf);
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }
};

// ─────────────────────────────────────────────
//  Compound tag  — named key/value store
// ─────────────────────────────────────────────
struct Compound : Tag {
    // Insertion-ordered entries
    std::vector<std::pair<std::string, TagPtr>> entries;

    Compound() = default;

    // Generic setter
    void set(const std::string& key, TagPtr tag) {
        for (auto& [k, v] : entries) {
            if (k == key) { v = std::move(tag); return; }
        }
        entries.emplace_back(key, std::move(tag));
    }

    // Convenience setters — no need to wrap manually
    void set(const std::string& key, int8_t  v) { set(key, std::make_shared<Byte>  (v)); }
    void set(const std::string& key, int16_t v) { set(key, std::make_shared<Short> (v)); }
    void set(const std::string& key, int32_t v) { set(key, std::make_shared<Int>   (v)); }
    void set(const std::string& key, int64_t v) { set(key, std::make_shared<Long>  (v)); }
    void set(const std::string& key, float   v) { set(key, std::make_shared<Float> (v)); }
    void set(const std::string& key, double  v) { set(key, std::make_shared<Double>(v)); }
    void set(const std::string& key, const std::string& v) { set(key, std::make_shared<String>(v)); }
    void set(const std::string& key, const char*        v) { set(key, std::make_shared<String>(std::string(v))); }

    // Retrieve a child compound by key (creates it if missing)
    std::shared_ptr<Compound> compound(const std::string& key) {
        for (auto& [k, v] : entries) {
            if (k == key) {
                auto c = std::dynamic_pointer_cast<Compound>(v);
                if (!c) throw std::runtime_error("NBT key '" + key + "' exists but is not a Compound");
                return c;
            }
        }
        auto c = std::make_shared<Compound>();
        set(key, c);
        return c;
    }

    // Retrieve a child list by key (creates it if missing)
    std::shared_ptr<List> list(const std::string& key, TagType elemType = TagType::End) {
        for (auto& [k, v] : entries) {
            if (k == key) {
                auto l = std::dynamic_pointer_cast<List>(v);
                if (!l) throw std::runtime_error("NBT key '" + key + "' exists but is not a List");
                return l;
            }
        }
        auto l = std::make_shared<List>(elemType);
        set(key, l);
        return l;
    }

    TagType type() const override { return TagType::Compound; }

    void writePayload(std::vector<uint8_t>& buf) const override {
        for (const auto& [key, tag] : entries) {
            // Write: tag type ID, name, payload
            detail::writeByte (buf, static_cast<uint8_t>(tag->type()));
            detail::writeString(buf, key);
            tag->writePayload(buf);
        }
        // TAG_End closes the compound
        detail::writeByte(buf, static_cast<uint8_t>(TagType::End));
    }
    void write(std::vector<uint8_t>& buf) const override { writePayload(buf); }

    // ── File I/O ──────────────────────────────────────────────────────────

    // Serialise to a raw byte buffer (uncompressed binary NBT)
    std::vector<uint8_t> toBytes(const std::string& rootName = "") const {
        std::vector<uint8_t> buf;
        // Root compound header: type + name
        detail::writeByte (buf, static_cast<uint8_t>(TagType::Compound));
        detail::writeString(buf, rootName);
        writePayload(buf);
        return buf;
    }

    // Write uncompressed binary NBT to file
    bool save(const std::string& path, const std::string& rootName = "") const {
        auto bytes = toBytes(rootName);
        std::ofstream f(path, std::ios::binary);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()));
        return f.good();
    }
};

} // namespace NBT

#endif // NBT_HPP
