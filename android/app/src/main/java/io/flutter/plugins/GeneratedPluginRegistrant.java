package io.flutter.plugins;

import io.flutter.plugin.common.PluginRegistry;
import com.cajian.nim_sdk_util.NimSdkUtilPlugin;
import io.flutter.plugins.sharedpreferences.SharedPreferencesPlugin;
import com.cajian.wx_sdk.WxSdkPlugin;

/**
 * Generated file. Do not edit.
 */
public final class GeneratedPluginRegistrant {
  public static void registerWith(PluginRegistry registry) {
    if (alreadyRegisteredWith(registry)) {
      return;
    }
    NimSdkUtilPlugin.registerWith(registry.registrarFor("com.cajian.nim_sdk_util.NimSdkUtilPlugin"));
    SharedPreferencesPlugin.registerWith(registry.registrarFor("io.flutter.plugins.sharedpreferences.SharedPreferencesPlugin"));
    WxSdkPlugin.registerWith(registry.registrarFor("com.cajian.wx_sdk.WxSdkPlugin"));
  }

  private static boolean alreadyRegisteredWith(PluginRegistry registry) {
    final String key = GeneratedPluginRegistrant.class.getCanonicalName();
    if (registry.hasPlugin(key)) {
      return true;
    }
    registry.registrarFor(key);
    return false;
  }
}
