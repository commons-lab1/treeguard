/**
 * TreeGuard Frame — Parametric OpenSCAD model
 *
 * Prints as modular segments:
 *   - 3x vertical spines (with electronics mount on one)
 *   - 3-4x hex ring segments (stackable for height)
 *   - 1x electronics enclosure
 *   - 3x ground stakes
 *   - 1x solar panel tilt mount (V1 only)
 *
 * Print in PETG. Do NOT use PLA for outdoor deployment.
 *
 * Usage:
 *   openscad -o spine.stl -D 'part="spine"' treeguard-frame.scad
 *   openscad -o ring.stl -D 'part="ring"' treeguard-frame.scad
 *   openscad -o enclosure.stl -D 'part="enclosure"' treeguard-frame.scad
 *   openscad -o stake.stl -D 'part="stake"' treeguard-frame.scad
 *   openscad -o assembly.stl -D 'part="assembly"' treeguard-frame.scad
 */

// === PARAMETERS (adjust to your tree) ===

// Inner diameter of the frame (mm) — clearance around trunk
inner_diameter = 200;  // [150:10:300]

// Total height (mm) — how tall the protective cage is
frame_height = 700;    // [400:50:1000]

// Number of ring levels
ring_count = 3;        // [2:1:5]

// Wall thickness of structural parts (mm)
wall = 4;              // [3:1:6]

// Spine width (mm)
spine_width = 20;      // [15:5:30]

// Ring bar width (mm)
ring_bar_width = 8;    // [6:1:12]

// Number of openings per ring segment (between spines)
ring_openings = 4;     // [3:1:6]

// M3 insert hole diameter (mm)
insert_d = 4.2;

// M3 screw clearance hole (mm)
screw_d = 3.4;

// Which part to render
part = "assembly";     // [assembly, spine, ring, enclosure, stake, solar_mount]

// === DERIVED ===

inner_r = inner_diameter / 2;
outer_r = inner_r + wall;
ring_spacing = frame_height / (ring_count + 1);
spine_angles = [0, 120, 240];

// === MODULES ===

module spine() {
    difference() {
        union() {
            // Main vertical bar
            translate([-spine_width/2, inner_r - wall/2, 0])
                cube([spine_width, wall, frame_height]);

            // Ring mounting tabs
            for (z = [1:ring_count]) {
                translate([0, inner_r, z * ring_spacing])
                    rotate([0, 0, 0])
                    cylinder(h = wall, d = spine_width, $fn = 6);
            }

            // Electronics mount clip (on one spine only)
            translate([-spine_width/2, inner_r - wall, frame_height * 0.7])
                cube([spine_width, wall * 3, 40]);
        }

        // M3 insert holes at each ring level
        for (z = [1:ring_count]) {
            translate([0, inner_r, z * ring_spacing - 0.1])
                cylinder(h = wall + 0.2, d = insert_d, $fn = 20);
        }

        // Ground stake socket at bottom
        translate([0, inner_r, -0.1])
            cylinder(h = 30.1, d = 10, $fn = 20);
    }
}

module ring_segment() {
    // 120-degree arc connecting two spine positions
    arc_angle = 120;
    bar_r = inner_r + wall/2;

    difference() {
        union() {
            // Curved bars (top and bottom of ring segment)
            for (offset = [-ring_bar_width/2, ring_bar_width/2]) {
                rotate_extrude(angle = arc_angle, $fn = 60)
                    translate([bar_r, offset, 0])
                    circle(d = wall, $fn = 12);
            }

            // Vertical bars between openings
            vert_count = ring_openings - 1;
            for (i = [1:vert_count]) {
                a = i * arc_angle / ring_openings;
                rotate([0, 0, a])
                    translate([bar_r, 0, -ring_bar_width/2])
                    cylinder(h = ring_bar_width, d = wall, $fn = 12);
            }

            // Mounting ears at 0 and 120 degrees
            for (a = [0, arc_angle]) {
                rotate([0, 0, a])
                    translate([bar_r, 0, 0])
                    cylinder(h = wall, d = spine_width * 0.8, $fn = 6);
            }
        }

        // Screw holes at mounting ears
        for (a = [0, arc_angle]) {
            rotate([0, 0, a])
                translate([bar_r, 0, -0.1])
                cylinder(h = wall + 0.2, d = screw_d, $fn = 20);
        }
    }
}

module electronics_enclosure() {
    // Box for PCB + battery, clips onto spine
    box_w = 60;
    box_d = 40;
    box_h = 30;
    lid_h = 5;

    difference() {
        union() {
            // Main box
            translate([-box_w/2, 0, 0])
                cube([box_w, box_d, box_h]);

            // Lid overlap rim
            translate([-(box_w - wall*2)/2, wall, box_h])
                cube([box_w - wall*2, box_d - wall*2, lid_h]);
        }

        // Hollow interior
        translate([-(box_w - wall*2)/2, wall, wall])
            cube([box_w - wall*2, box_d - wall*2, box_h]);

        // Cable gland holes (sensor wire, LED wires)
        for (x = [-15, 0, 15]) {
            translate([x, -0.1, box_h/2])
                rotate([-90, 0, 0])
                cylinder(h = wall + 0.2, d = 6, $fn = 20);
        }

        // Gasket groove in lid rim
        translate([-(box_w - wall*4)/2, wall*2, box_h - 0.5])
            cube([box_w - wall*4, box_d - wall*4, 1]);
    }

    // Spine clip (C-shaped, snaps around spine)
    translate([-spine_width/2 - 2, -wall - 2, 0])
        difference() {
            cube([spine_width + 4, wall + 2, box_h]);
            translate([2, -0.1, 2])
                cube([spine_width, wall + 0.2, box_h - 4]);
        }
}

module ground_stake() {
    // Tapered stake that fits into spine socket
    stake_d = 9.5;  // slight clearance for 10mm socket
    stake_h = 200;

    union() {
        // Socket insert portion
        cylinder(h = 28, d = stake_d, $fn = 20);

        // Below-ground tapered shaft
        translate([0, 0, -stake_h])
            cylinder(h = stake_h, d1 = 3, d2 = stake_d, $fn = 20);
    }
}

module solar_mount() {
    // 30-degree tilt bracket for 5V solar panel (~55x55mm)
    panel_w = 58;
    panel_d = 58;
    tilt = 30;

    // Base plate (mounts on top of spine)
    translate([-spine_width/2, 0, 0])
        cube([spine_width, wall, 5]);

    // Tilted platform
    rotate([tilt, 0, 0])
        translate([-panel_w/2, 0, 5]) {
            difference() {
                cube([panel_w, panel_d, 3]);
                // Panel retention lips
                translate([2, 2, 1.5])
                    cube([panel_w - 4, panel_d - 4, 2]);
            }
        }
}

// === ASSEMBLY / PART SELECTION ===

if (part == "spine") {
    spine();
}
else if (part == "ring") {
    ring_segment();
}
else if (part == "enclosure") {
    electronics_enclosure();
}
else if (part == "stake") {
    ground_stake();
}
else if (part == "solar_mount") {
    solar_mount();
}
else if (part == "assembly") {
    // Full assembly preview
    color("DarkOliveGreen") {
        for (a = spine_angles) {
            rotate([0, 0, a])
                spine();
        }
    }

    color("ForestGreen", 0.7) {
        for (z = [1:ring_count]) {
            for (a = spine_angles) {
                translate([0, 0, z * ring_spacing])
                    rotate([0, 0, a])
                    ring_segment();
            }
        }
    }

    color("DimGray") {
        rotate([0, 0, 0])
            translate([0, inner_r + wall, frame_height * 0.7])
            electronics_enclosure();
    }

    color("SteelBlue") {
        rotate([0, 0, 0])
            translate([0, inner_r + wall, frame_height + 5])
            solar_mount();
    }

    color("SaddleBrown", 0.5) {
        for (a = spine_angles) {
            rotate([0, 0, a])
                translate([0, inner_r, 0])
                ground_stake();
        }
    }
}
