//===========================================================================
//
// File: istl_test.cpp
//
// Created: Mon Sep 21 13:41:16 2009
//
// Author(s): Atgeirr F Rasmussen <atgeirr@sintef.no>
//            B�rd Skaflestad     <bard.skaflestad@sintef.no>
//
// $Date$
//
// $Revision$
//
//===========================================================================

/*
  Copyright 2009, 2010 SINTEF ICT, Applied Mathematics.
  Copyright 2009, 2010 Statoil ASA.

  This file is part of The Open Reservoir Simulator Project (OpenRS).

  OpenRS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenRS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenRS.  If not, see <http://www.gnu.org/licenses/>.
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif


#if HAVE_MPI // #else clause at bottom of file



#include <dune/istl/bvector.hh>
#include <dune/istl/bcrsmatrix.hh>
#include <dune/istl/operators.hh>
#include <dune/istl/io.hh>
#include <dune/istl/indexset.hh>
#include <dune/istl/overlappingschwarz.hh>
#include <dune/istl/schwarz.hh>
#include <dune/istl/preconditioners.hh>
#include <dune/istl/solvers.hh>
#include <dune/istl/owneroverlapcopy.hh>
#include <dune/istl/paamg/amg.hh>
#include <dune/istl/paamg/pinfo.hh>
#include <dune/common/mpihelper.hh>


const int ltg_size = 1081;
const int local_to_global[ltg_size] = {
    359, 0, 360, 361, 1, 362, 363, 2, 364, 365, 3, 366, 367, 4, 368, 369, 5, 370, 371, 6, 372, 373, 7, 374, 375, 8, 376, 377, 9, 378, 379, 10, 380, 381, 11, 382, 383, 12, 384, 385, 13, 386, 387, 14, 388, 389, 15, 390, 391, 16, 392, 393, 17, 394, 395, 18, 396, 397, 19, 398, 399, 20, 400, 401, 21, 402, 403, 22, 404, 405, 23, 406, 407, 24, 408, 409, 25, 410, 411, 26, 412, 413, 27, 414, 415, 28, 416, 417, 29, 418, 419, 30, 420, 421, 31, 422, 423, 32, 424, 425, 33, 426, 427, 34, 428, 429, 35, 430, 431, 36, 432, 433, 37, 434, 435, 38, 436, 437, 39, 438, 439, 40, 440, 441, 41, 442, 443, 42, 444, 445, 43, 446, 447, 44, 448, 449, 45, 450, 451, 46, 452, 453, 47, 454, 455, 48, 456, 457, 49, 458, 459, 50, 460, 461, 51, 462, 463, 52, 464, 465, 53, 466, 467, 54, 468, 469, 55, 470, 471, 56, 472, 473, 57, 474, 475, 58, 476, 477, 59, 478, 479, 60, 480, 481, 61, 482, 483, 62, 484, 485, 63, 486, 487, 64, 488, 489, 65, 490, 491, 66, 492, 493, 67, 494, 495, 68, 496, 497, 69, 498, 499, 70, 500, 501, 71, 502, 503, 72, 504, 505, 73, 506, 507, 74, 508, 509, 75, 510, 511, 76, 512, 513, 77, 514, 515, 78, 516, 517, 79, 518, 519, 80, 520, 521, 81, 522, 523, 82, 524, 525, 83, 526, 527, 84, 528, 529, 85, 530, 531, 86, 532, 533, 87, 534, 535, 88, 536, 537, 89, 538, 539, 90, 540, 541, 91, 542, 543, 92, 544, 545, 93, 546, 547, 94, 548, 549, 95, 550, 551, 96, 552, 553, 97, 554, 555, 98, 556, 557, 99, 558, 559, 100, 560, 561, 101, 562, 563, 102, 564, 565, 103, 566, 567, 104, 568, 569, 105, 570, 571, 106, 572, 573, 107, 574, 575, 108, 576, 577, 109, 578, 579, 110, 580, 581, 111, 582, 583, 112, 584, 585, 113, 586, 587, 114, 588, 589, 115, 590, 591, 116, 592, 593, 117, 594, 595, 118, 596, 597, 119, 598, 599, 120, 600, 601, 121, 602, 603, 122, 604, 605, 123, 606, 607, 124, 608, 609, 125, 610, 611, 126, 612, 613, 127, 614, 615, 128, 616, 617, 129, 618, 619, 130, 620, 621, 131, 622, 623, 132, 624, 625, 133, 626, 627, 134, 628, 629, 135, 630, 631, 136, 632, 633, 137, 634, 635, 138, 636, 637, 139, 638, 639, 140, 640, 641, 141, 642, 643, 142, 644, 645, 143, 646, 647, 144, 648, 649, 145, 650, 651, 146, 652, 653, 147, 654, 655, 148, 656, 657, 149, 658, 659, 150, 660, 661, 151, 662, 663, 152, 664, 665, 153, 666, 667, 154, 668, 669, 155, 670, 671, 156, 672, 673, 157, 674, 675, 158, 676, 677, 159, 678, 679, 160, 680, 681, 161, 682, 683, 162, 684, 685, 163, 686, 687, 164, 688, 689, 165, 690, 691, 166, 692, 693, 167, 694, 695, 168, 696, 697, 169, 698, 699, 170, 700, 701, 171, 702, 703, 172, 704, 705, 173, 706, 707, 174, 708, 709, 175, 710, 711, 176, 712, 713, 177, 714, 715, 178, 716, 717, 179, 718, 719, 180, 720, 721, 181, 722, 723, 182, 724, 725, 183, 726, 727, 184, 728, 729, 185, 730, 731, 186, 732, 733, 187, 734, 735, 188, 736, 737, 189, 738, 739, 190, 740, 741, 191, 742, 743, 192, 744, 745, 193, 746, 747, 194, 748, 749, 195, 750, 751, 196, 752, 753, 197, 754, 755, 198, 756, 757, 199, 758, 759, 200, 760, 761, 201, 762, 763, 202, 764, 765, 203, 766, 767, 204, 768, 769, 205, 770, 771, 206, 772, 773, 207, 774, 775, 208, 776, 777, 209, 778, 779, 210, 780, 781, 211, 782, 783, 212, 784, 785, 213, 786, 787, 214, 788, 789, 215, 790, 791, 216, 792, 793, 217, 794, 795, 218, 796, 797, 219, 798, 799, 220, 800, 801, 221, 802, 803, 222, 804, 805, 223, 806, 807, 224, 808, 809, 225, 810, 811, 226, 812, 813, 227, 814, 815, 228, 816, 817, 229, 818, 819, 230, 820, 821, 231, 822, 823, 232, 824, 825, 233, 826, 827, 234, 828, 829, 235, 830, 831, 236, 832, 833, 237, 834, 835, 238, 836, 837, 239, 838, 839, 240, 840, 841, 241, 842, 843, 242, 844, 845, 243, 846, 847, 244, 848, 849, 245, 850, 851, 246, 852, 853, 247, 854, 855, 248, 856, 857, 249, 858, 859, 250, 860, 861, 251, 862, 863, 252, 864, 865, 253, 866, 867, 254, 868, 869, 255, 870, 871, 256, 872, 873, 257, 874, 875, 258, 876, 877, 259, 878, 879, 260, 880, 881, 261, 882, 883, 262, 884, 885, 263, 886, 887, 264, 888, 889, 265, 890, 891, 266, 892, 893, 267, 894, 895, 268, 896, 897, 269, 898, 899, 270, 900, 901, 271, 902, 903, 272, 904, 905, 273, 906, 907, 274, 908, 909, 275, 910, 911, 276, 912, 913, 277, 914, 915, 278, 916, 917, 279, 918, 919, 280, 920, 921, 281, 922, 923, 282, 924, 925, 283, 926, 927, 284, 928, 929, 285, 930, 931, 286, 932, 933, 287, 934, 935, 288, 936, 937, 289, 938, 939, 290, 940, 941, 291, 942, 943, 292, 944, 945, 293, 946, 947, 294, 948, 949, 295, 950, 951, 296, 952, 953, 297, 954, 955, 298, 956, 957, 299, 958, 959, 300, 960, 961, 301, 962, 963, 302, 964, 965, 303, 966, 967, 304, 968, 969, 305, 970, 971, 306, 972, 973, 307, 974, 975, 308, 976, 977, 309, 978, 979, 310, 980, 981, 311, 982, 983, 312, 984, 985, 313, 986, 987, 314, 988, 989, 315, 990, 991, 316, 992, 993, 317, 994, 995, 318, 996, 997, 319, 998, 999, 320, 1000, 1001, 321, 1002, 1003, 322, 1004, 1005, 323, 1006, 1007, 324, 1008, 1009, 325, 1010, 1011, 326, 1012, 1013, 327, 1014, 1015, 328, 1016, 1017, 329, 1018, 1019, 330, 1020, 1021, 331, 1022, 1023, 332, 1024, 1025, 333, 1026, 1027, 334, 1028, 1029, 335, 1030, 1031, 336, 1032, 1033, 337, 1034, 1035, 338, 1036, 1037, 339, 1038, 1039, 340, 1040, 1041, 341, 1042, 1043, 342, 1044, 1045, 343, 1046, 1047, 344, 1048, 1049, 345, 1050, 1051, 346, 1052, 1053, 347, 1054, 1055, 348, 1056, 1057, 349, 1058, 1059, 350, 1060, 1061, 351, 1062, 1063, 352, 1064, 1065, 353, 1066, 1067, 354, 1068, 1069, 355, 1070, 1071, 356, 1072, 1073, 357, 1074, 1075, 358, 1076, 1077, 1078, 1079, 1080 };


int main(int argc, char** argv)
{
    int mpi_rank = Dune::MPIHelper::instance(argc,argv).rank();
    int mpi_size = Dune::MPIHelper::instance(argc,argv).size();
    std::cout << "Hello from rank " << mpi_rank << "   total mpi size is " << mpi_size << std::endl;

    typedef int LocalId;
    typedef int GlobalId;
    typedef Dune::OwnerOverlapCopyCommunication<LocalId,GlobalId> Communication;
    typedef Dune::OwnerOverlapCopyAttributeSet GridAttributes;
    typedef GridAttributes::AttributeSet GridFlag;
    typedef Dune::ParallelLocalIndex<GridFlag> LocalIndex;
    Communication comm;

    Communication::PIS& pis = comm.indexSet();
    pis.beginResize();
    for (int i = 0; i < ltg_size; ++i) {
	GridFlag flag = GridAttributes::owner;
	bool is_public = false;
	pis.add(local_to_global[i], LocalIndex(i, flag, is_public));
    }
    pis.endResize();
}




#else

// We do not have MPI

#include <iostream>

int main()
{
    std::cerr << "This program does nothing if HAVE_MPI is undefined.\n"
	"To enable MPI, pass --enable-parallel to configure (or dunecontrol) when setting up dune.\n";
}


#endif