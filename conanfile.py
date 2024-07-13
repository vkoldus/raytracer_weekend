# This file is managed by Conan, contents will be overwritten.
# To keep your changes, remove these comment lines, but the plugin won't be able to modify your requirements

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain
from conan.tools.files import copy
import os


class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
                                          "res", "bindings"), os.path.join(self.source_folder, "src", "bindings"))
        copy(self, "*opengl2*", os.path.join(self.dependencies["imgui"].package_folder,
                                             "res", "bindings"), os.path.join(self.source_folder, "src", "bindings"))

    def requirements(self):
        requirements = self.conan_data.get('requirements', [])
        for requirement in requirements:
            self.requires(requirement)
