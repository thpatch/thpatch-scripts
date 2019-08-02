using System;
using System.IO;

namespace UnVLV
{
    class Program
    {
        static readonly byte[] MZHeader = new byte[0x3C] {
            0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
            0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
        };

        static readonly byte[] DOSStub = new byte[0x40] {
            0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD,
            0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21, 0x54, 0x68,
            0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72,
            0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F,
            0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6E,
            0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
            0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A,
            0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        };

        static bool MatchBlob(byte[] blob, byte[] data, long offset)
        {
            if (offset + blob.Length > data.Length)
                return false;
            for (int i = 0; i < blob.Length; i++) {
                if (blob[i] != data[offset + i])
                    return false;
            }
            return true;
        }
        static void Main(string[] args)
        {
            if (args.Length != 1) {
                Console.WriteLine("Usage: UnVLV unpacked.exe");
                return;
            }
            string fn = args[0].Trim('"');
            byte[] contents = File.ReadAllBytes(fn);
            if (!MatchBlob(MZHeader, contents, 0)) {
                Console.WriteLine("MZ header doesn't match the MS LINK one.");
                return;
            }
            uint e_lfanew;
            if (BitConverter.IsLittleEndian) {
                e_lfanew = BitConverter.ToUInt32(contents, 0x3C);
            } else {
                e_lfanew = BitConverter.ToUInt32(
                    new byte[] { contents[0x3F], contents[0x3E], contents[0x3F], contents[0x3C] }, 0);
            }
            if (e_lfanew > contents.Length) {
                Console.WriteLine("e_lfanew out of bounds.");
                return;
            }
            // TODO: what does steam do when it's less than 0xD0?
            if (e_lfanew < 0x80) {
                Console.WriteLine("DOS stub space is too small.");
                return;
            }

            DOSStub.CopyTo(contents, 0x40);
            for (int i = 0x80; i < e_lfanew; i++) {
                contents[i] = 0x00;
            }

            string newfn;
            if (fn.ToLowerInvariant().EndsWith(".unpacked.exe")) {
                newfn = fn.Substring(0, fn.Length - 13) + ".unvlv.exe";
            } else {
                newfn = fn + ".unvlv.exe";
            }
            File.WriteAllBytes(newfn, contents);
            Console.WriteLine("Done.");
        }
    }
}
