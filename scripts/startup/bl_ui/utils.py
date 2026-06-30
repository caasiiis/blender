# SPDX-FileCopyrightText: 2009-2023 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

from bpy.types import Menu


# Shared render engine compatibility sets.
# Use these instead of repeating the engine name literals in each panel class.
COMPAT_ENGINES_ALL = frozenset({
    'BLENDER_RENDER',
    'BLENDER_EEVEE',
    'BLENDER_WORKBENCH',
})
COMPAT_ENGINES_EEVEE = frozenset({'BLENDER_EEVEE'})
COMPAT_ENGINES_WORKBENCH = frozenset({'BLENDER_WORKBENCH'})
COMPAT_ENGINES_EEVEE_WORKBENCH = frozenset({
    'BLENDER_EEVEE',
    'BLENDER_WORKBENCH',
})
COMPAT_ENGINES_RENDER_WORKBENCH = frozenset({
    'BLENDER_RENDER',
    'BLENDER_WORKBENCH',
})
COMPAT_ENGINES_RENDER_EEVEE = frozenset({
    'BLENDER_RENDER',
    'BLENDER_EEVEE',
})


# Base mix-in for data property panels (don't register).
# Subclasses must override ``poll`` to check the relevant context attribute.
class DataButtonsPanelBase:
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "data"


# Panel mix-in class (don't register).
class PresetPanel:
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'HEADER'
    bl_label = "Presets"
    path_menu = Menu.path_menu

    @classmethod
    def draw_panel_header(cls, layout):
        layout.emboss = 'NONE'
        layout.popover(
            panel=cls.__name__,
            icon='PRESET',
            text="",
        )

    @classmethod
    def draw_menu(cls, layout, text=None):
        if text is None:
            text = cls.bl_label

        layout.popover(
            panel=cls.__name__,
            icon='PRESET',
            text=text,
        )

    def draw(self, context):
        layout = self.layout
        layout.emboss = 'PULLDOWN_MENU'
        layout.operator_context = 'EXEC_DEFAULT'

        Menu.draw_preset(self, context)
