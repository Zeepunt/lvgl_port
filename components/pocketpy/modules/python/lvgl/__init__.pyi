# __init__.pyi
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2023-2026 Zeepunt

class lvgl_obj_t:
    """
    Base LVGL object
    """
    pass

# Button
def btn_create(parent: lvgl_obj_t | None = None) -> lvgl_obj_t:
    """
    Create a button object.

    Args:
        parent:
            Parent LVGL object.
            If None, the button will be created on the active screen.

    Returns:
        Created button object.

    Raises:
        RuntimeError:
            Failed to create button object.
    """
    pass