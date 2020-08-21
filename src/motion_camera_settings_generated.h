// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MOTIONCAMERASETTINGS_OV5642_SETTINGS_H_
#define FLATBUFFERS_GENERATED_MOTIONCAMERASETTINGS_OV5642_SETTINGS_H_

#include "flatbuffers/flatbuffers.h"

namespace OV5642_Settings {

struct Settings;
struct SettingsBuilder;

struct Settings FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SettingsBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_JPEG_SIZE = 4,
    VT_QUALITY = 6,
    VT_FRAMES = 8,
    VT_LIGHT_MODE = 10,
    VT_COLOR_SATURATION = 12,
    VT_BRIGHTNESS = 14,
    VT_CONTRAST = 16,
    VT_HUE = 18,
    VT_EFFECT = 20,
    VT_EXPOSURE = 22,
    VT_SHARPNESS = 24,
    VT_MIRROR = 26,
    VT_TEST_PATTERN = 28
  };
  uint8_t jpeg_size() const {
    return GetField<uint8_t>(VT_JPEG_SIZE, 0);
  }
  bool mutate_jpeg_size(uint8_t _jpeg_size) {
    return SetField<uint8_t>(VT_JPEG_SIZE, _jpeg_size, 0);
  }
  uint8_t quality() const {
    return GetField<uint8_t>(VT_QUALITY, 0);
  }
  bool mutate_quality(uint8_t _quality) {
    return SetField<uint8_t>(VT_QUALITY, _quality, 0);
  }
  uint8_t frames() const {
    return GetField<uint8_t>(VT_FRAMES, 0);
  }
  bool mutate_frames(uint8_t _frames) {
    return SetField<uint8_t>(VT_FRAMES, _frames, 0);
  }
  uint8_t light_mode() const {
    return GetField<uint8_t>(VT_LIGHT_MODE, 0);
  }
  bool mutate_light_mode(uint8_t _light_mode) {
    return SetField<uint8_t>(VT_LIGHT_MODE, _light_mode, 0);
  }
  uint8_t color_saturation() const {
    return GetField<uint8_t>(VT_COLOR_SATURATION, 0);
  }
  bool mutate_color_saturation(uint8_t _color_saturation) {
    return SetField<uint8_t>(VT_COLOR_SATURATION, _color_saturation, 0);
  }
  uint8_t brightness() const {
    return GetField<uint8_t>(VT_BRIGHTNESS, 0);
  }
  bool mutate_brightness(uint8_t _brightness) {
    return SetField<uint8_t>(VT_BRIGHTNESS, _brightness, 0);
  }
  uint8_t contrast() const {
    return GetField<uint8_t>(VT_CONTRAST, 0);
  }
  bool mutate_contrast(uint8_t _contrast) {
    return SetField<uint8_t>(VT_CONTRAST, _contrast, 0);
  }
  uint8_t hue() const {
    return GetField<uint8_t>(VT_HUE, 0);
  }
  bool mutate_hue(uint8_t _hue) {
    return SetField<uint8_t>(VT_HUE, _hue, 0);
  }
  uint8_t effect() const {
    return GetField<uint8_t>(VT_EFFECT, 0);
  }
  bool mutate_effect(uint8_t _effect) {
    return SetField<uint8_t>(VT_EFFECT, _effect, 0);
  }
  uint8_t exposure() const {
    return GetField<uint8_t>(VT_EXPOSURE, 0);
  }
  bool mutate_exposure(uint8_t _exposure) {
    return SetField<uint8_t>(VT_EXPOSURE, _exposure, 0);
  }
  uint8_t sharpness() const {
    return GetField<uint8_t>(VT_SHARPNESS, 0);
  }
  bool mutate_sharpness(uint8_t _sharpness) {
    return SetField<uint8_t>(VT_SHARPNESS, _sharpness, 0);
  }
  uint8_t mirror() const {
    return GetField<uint8_t>(VT_MIRROR, 0);
  }
  bool mutate_mirror(uint8_t _mirror) {
    return SetField<uint8_t>(VT_MIRROR, _mirror, 0);
  }
  uint8_t test_pattern() const {
    return GetField<uint8_t>(VT_TEST_PATTERN, 0);
  }
  bool mutate_test_pattern(uint8_t _test_pattern) {
    return SetField<uint8_t>(VT_TEST_PATTERN, _test_pattern, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_JPEG_SIZE) &&
           VerifyField<uint8_t>(verifier, VT_QUALITY) &&
           VerifyField<uint8_t>(verifier, VT_FRAMES) &&
           VerifyField<uint8_t>(verifier, VT_LIGHT_MODE) &&
           VerifyField<uint8_t>(verifier, VT_COLOR_SATURATION) &&
           VerifyField<uint8_t>(verifier, VT_BRIGHTNESS) &&
           VerifyField<uint8_t>(verifier, VT_CONTRAST) &&
           VerifyField<uint8_t>(verifier, VT_HUE) &&
           VerifyField<uint8_t>(verifier, VT_EFFECT) &&
           VerifyField<uint8_t>(verifier, VT_EXPOSURE) &&
           VerifyField<uint8_t>(verifier, VT_SHARPNESS) &&
           VerifyField<uint8_t>(verifier, VT_MIRROR) &&
           VerifyField<uint8_t>(verifier, VT_TEST_PATTERN) &&
           verifier.EndTable();
  }
};

struct SettingsBuilder {
  typedef Settings Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_jpeg_size(uint8_t jpeg_size) {
    fbb_.AddElement<uint8_t>(Settings::VT_JPEG_SIZE, jpeg_size, 0);
  }
  void add_quality(uint8_t quality) {
    fbb_.AddElement<uint8_t>(Settings::VT_QUALITY, quality, 0);
  }
  void add_frames(uint8_t frames) {
    fbb_.AddElement<uint8_t>(Settings::VT_FRAMES, frames, 0);
  }
  void add_light_mode(uint8_t light_mode) {
    fbb_.AddElement<uint8_t>(Settings::VT_LIGHT_MODE, light_mode, 0);
  }
  void add_color_saturation(uint8_t color_saturation) {
    fbb_.AddElement<uint8_t>(Settings::VT_COLOR_SATURATION, color_saturation, 0);
  }
  void add_brightness(uint8_t brightness) {
    fbb_.AddElement<uint8_t>(Settings::VT_BRIGHTNESS, brightness, 0);
  }
  void add_contrast(uint8_t contrast) {
    fbb_.AddElement<uint8_t>(Settings::VT_CONTRAST, contrast, 0);
  }
  void add_hue(uint8_t hue) {
    fbb_.AddElement<uint8_t>(Settings::VT_HUE, hue, 0);
  }
  void add_effect(uint8_t effect) {
    fbb_.AddElement<uint8_t>(Settings::VT_EFFECT, effect, 0);
  }
  void add_exposure(uint8_t exposure) {
    fbb_.AddElement<uint8_t>(Settings::VT_EXPOSURE, exposure, 0);
  }
  void add_sharpness(uint8_t sharpness) {
    fbb_.AddElement<uint8_t>(Settings::VT_SHARPNESS, sharpness, 0);
  }
  void add_mirror(uint8_t mirror) {
    fbb_.AddElement<uint8_t>(Settings::VT_MIRROR, mirror, 0);
  }
  void add_test_pattern(uint8_t test_pattern) {
    fbb_.AddElement<uint8_t>(Settings::VT_TEST_PATTERN, test_pattern, 0);
  }
  explicit SettingsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Settings> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Settings>(end);
    return o;
  }
};

inline flatbuffers::Offset<Settings> CreateSettings(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t jpeg_size = 0,
    uint8_t quality = 0,
    uint8_t frames = 0,
    uint8_t light_mode = 0,
    uint8_t color_saturation = 0,
    uint8_t brightness = 0,
    uint8_t contrast = 0,
    uint8_t hue = 0,
    uint8_t effect = 0,
    uint8_t exposure = 0,
    uint8_t sharpness = 0,
    uint8_t mirror = 0,
    uint8_t test_pattern = 0) {
  SettingsBuilder builder_(_fbb);
  builder_.add_test_pattern(test_pattern);
  builder_.add_mirror(mirror);
  builder_.add_sharpness(sharpness);
  builder_.add_exposure(exposure);
  builder_.add_effect(effect);
  builder_.add_hue(hue);
  builder_.add_contrast(contrast);
  builder_.add_brightness(brightness);
  builder_.add_color_saturation(color_saturation);
  builder_.add_light_mode(light_mode);
  builder_.add_frames(frames);
  builder_.add_quality(quality);
  builder_.add_jpeg_size(jpeg_size);
  return builder_.Finish();
}

inline const OV5642_Settings::Settings *GetSettings(const void *buf) {
  return flatbuffers::GetRoot<OV5642_Settings::Settings>(buf);
}

inline const OV5642_Settings::Settings *GetSizePrefixedSettings(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<OV5642_Settings::Settings>(buf);
}

inline Settings *GetMutableSettings(void *buf) {
  return flatbuffers::GetMutableRoot<Settings>(buf);
}

inline bool VerifySettingsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<OV5642_Settings::Settings>(nullptr);
}

inline bool VerifySizePrefixedSettingsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<OV5642_Settings::Settings>(nullptr);
}

inline void FinishSettingsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<OV5642_Settings::Settings> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedSettingsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<OV5642_Settings::Settings> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace OV5642_Settings

#endif  // FLATBUFFERS_GENERATED_MOTIONCAMERASETTINGS_OV5642_SETTINGS_H_