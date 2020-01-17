// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Collections.Generic;
using Tools.DotNETCommon;
using System.Security.Cryptography;
using System.Text;
using UnrealBuildTool;

public class ProtobufConfig
{
    FileReference LocationConfigRef;
    Dictionary<string, ConfigFileSection> Sections = new Dictionary<string, ConfigFileSection>(StringComparer.InvariantCultureIgnoreCase);

    public ProtobufConfig(FileReference Location, ConfigLineAction DefaultAction = ConfigLineAction.Set)
    {
        using (StreamReader Reader = new StreamReader(Location.FullName))
        {
            ConfigFileSection CurrentSection = null;
            while (!Reader.EndOfStream)
            {
                // Find the first non-whitespace character
                string Line = Reader.ReadLine();
                for (int StartIdx = 0; StartIdx < Line.Length; StartIdx++)
                {
                    if (Line[StartIdx] != ' ' && Line[StartIdx] != '\t')
                    {
                        // Find the last non-whitespace character. If it's an escaped newline, merge the following line with it.
                        int EndIdx = Line.Length;
                        for (; EndIdx > StartIdx; EndIdx--)
                        {
                            if (Line[EndIdx - 1] == '\\')
                            {
                                string NextLine = Reader.ReadLine();
                                if (NextLine == null)
                                {
                                    break;
                                }
                                Line += NextLine;
                                EndIdx = Line.Length;
                            }
                            if (Line[EndIdx - 1] != ' ' && Line[EndIdx - 1] != '\t')
                            {
                                break;
                            }
                        }

                        // Break out if we've got a comment
                        if (Line[StartIdx] == ';')
                        {
                            break;
                        }
                        if (Line[StartIdx] == '/' && StartIdx + 1 < Line.Length && Line[StartIdx + 1] == '/')
                        {
                            break;
                        }

                        // Check if it's the start of a new section
                        if (Line[StartIdx] == '[')
                        {
                            CurrentSection = null;
                            if (Line[EndIdx - 1] == ']')
                            {
                                string SectionName = Line.Substring(StartIdx + 1, EndIdx - StartIdx - 2);
                                if (!Sections.TryGetValue(SectionName, out CurrentSection))
                                {
                                    CurrentSection = new ConfigFileSection(SectionName);
                                    Sections.Add(SectionName, CurrentSection);
                                }
                            }
                            break;
                        }

                        // Otherwise add it to the current section or add a new one
                        if (CurrentSection != null)
                        {
                            if (!TryAddConfigLine(CurrentSection, Line, StartIdx, EndIdx, DefaultAction))
                            {
                                Log.TraceWarning("Couldn't parse '{0}' in {1} of {2}", Line, CurrentSection, Location.FullName);
                            }
                            break;
                        }

                        // Otherwise just ignore it
                        break;
                    }
                }
            }

            if(LocationConfigRef != Location)
                LocationConfigRef = Location;
        }
    }

    public static ProtobufConfig MakeInstance(string FilePath, ConfigLineAction DefaultAction = ConfigLineAction.Set)
    {
        FileReference ConfigRef = new FileReference(FilePath);
        if (FileReference.Exists(ConfigRef))
        {
            return new ProtobufConfig(ConfigRef, DefaultAction);
        }

        return null;
    }

    public static ProtobufConfig MakeInstance(string RootPath, string FileName, ConfigLineAction DefaultAction = ConfigLineAction.Set)
    {
        string FilePath = Path.Combine(RootPath, FileName).Replace('\\', '/');
        return MakeInstance(FilePath, DefaultAction);
    }

    static bool TryAddConfigLine(ConfigFileSection Section, string Line, int StartIdx, int EndIdx, ConfigLineAction DefaultAction)
    {
        // Find the '=' character separating key and value
        int EqualsIdx = Line.IndexOf('=', StartIdx, EndIdx - StartIdx);
        if (EqualsIdx == -1 && Line[StartIdx] != '!')
        {
            return false;
        }

        // Keep track of the start of the key name
        int KeyStartIdx = StartIdx;

        // Remove the +/-/! prefix, if present
        ConfigLineAction Action = DefaultAction;
        if (Line[KeyStartIdx] == '+' || Line[KeyStartIdx] == '-' || Line[KeyStartIdx] == '!')
        {
            Action = (Line[KeyStartIdx] == '+') ? ConfigLineAction.Add : (Line[KeyStartIdx] == '!') ? ConfigLineAction.RemoveKey : ConfigLineAction.RemoveKeyValue;
            KeyStartIdx++;
            while (Line[KeyStartIdx] == ' ' || Line[KeyStartIdx] == '\t')
            {
                KeyStartIdx++;
            }
        }

        // RemoveKey actions do not require a value
        if (Action == ConfigLineAction.RemoveKey && EqualsIdx == -1)
        {
            Section.Lines.Add(new ConfigLine(Action, Line.Substring(KeyStartIdx).Trim(), ""));
            return true;
        }

        // Remove trailing spaces after the name of the key
        int KeyEndIdx = EqualsIdx;
        for (; KeyEndIdx > KeyStartIdx; KeyEndIdx--)
        {
            if (Line[KeyEndIdx - 1] != ' ' && Line[KeyEndIdx - 1] != '\t')
            {
                break;
            }
        }

        // Make sure there's a non-empty key name
        if (KeyStartIdx == EqualsIdx)
        {
            return false;
        }

        // Skip whitespace between the '=' and the start of the value
        int ValueStartIdx = EqualsIdx + 1;
        for (; ValueStartIdx < EndIdx; ValueStartIdx++)
        {
            if (Line[ValueStartIdx] != ' ' && Line[ValueStartIdx] != '\t')
            {
                break;
            }
        }

        // Strip quotes around the value if present
        int ValueEndIdx = EndIdx;
        if (ValueEndIdx >= ValueStartIdx + 2 && Line[ValueStartIdx] == '"' && Line[ValueEndIdx - 1] == '"')
        {
            ValueStartIdx++;
            ValueEndIdx--;
        }

        // Add it to the config section
        string Key = Line.Substring(KeyStartIdx, KeyEndIdx - KeyStartIdx);
        string Value = Line.Substring(ValueStartIdx, ValueEndIdx - ValueStartIdx);
        Section.Lines.Add(new ConfigLine(Action, Key, Value));
        return true;
    }

    private bool TryGetSection(string SectionName, out ConfigFileSection RawSection)
    {
        return Sections.TryGetValue(SectionName, out RawSection);
    }

    public string GetValue(string SectionName, string inKey)
    {
        ConfigFileSection StageSettings;
        if (TryGetSection(SectionName, out StageSettings))
        {
            foreach (ConfigLine Line in StageSettings.Lines)
            {
                if (Line.Key == inKey)
                {
                    return Line.Value;
                }
            }
        }

        return null;
    }

    /*已经存在的Key则覆盖其value*/
    public void AddValue(string inSectionName, string inKey, string inValue)
    {
        ConfigFileSection CurrentSection = null;
        if (!TryGetSection(inSectionName, out CurrentSection))
        {
            CurrentSection = new ConfigFileSection(inSectionName);
            Sections.Add(inSectionName, CurrentSection);
        }

        if (CurrentSection != null)
        {
            bool bIsHaved = false;
            foreach (ConfigLine Line in CurrentSection.Lines)
            {
                if (Line.Key == inKey)
                {
                    Line.Value = inValue;
                    bIsHaved = true;
                }
            }

            if(!bIsHaved)
            {
                string IniKeyValue = string.Format("{0}={1}", inKey, inValue);
                if (!TryAddConfigLine(CurrentSection, IniKeyValue, 0, IniKeyValue.Length, ConfigLineAction.Set))
                {
                    System.Console.WriteLine("Couldn't parse '{0}'", IniKeyValue);
                }
            }
            
        }
    }

    public void WriteAs(FileReference Location)
    {
        using (StreamWriter Writer = new StreamWriter(Location.FullName))
        {
            foreach (ConfigFileSection Section in Sections.Values)
            {
                Writer.WriteLine("[{0}]", Section.Name);
                foreach (ConfigLine Line in Section.Lines)
                {
                    Writer.WriteLine("{0}", Line.ToString());
                }
                Writer.WriteLine();
            }
        }
    }

    public void Write()
    {
        WriteAs(LocationConfigRef);
    }


    private static string EncryptWithMD5(string source)
    {
        MD5 md5 = MD5.Create();
        byte[] result = md5.ComputeHash(Encoding.UTF8.GetBytes(source));

        return Convert.ToBase64String(result);
    }

    public static string HashMd5(string filepath)
    {
        using (StreamReader Reader = new StreamReader(filepath))
        {
            string source = Reader.ReadToEnd();
            return EncryptWithMD5(source);
        }
    }
}

public class ProtobufEditor : ModuleRules
{

    private string ProtocExe
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "bin/protoc.exe")).Replace('\\', '/'); }
    }

    private string ProtoPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "Proto/")).Replace('\\', '/'); }
    }

    private string SourcePath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../Protobuf/Protoc/")).Replace('\\', '/'); }
    }

    /**检查c文件是否存在,return false: 不存在，return true: 存在**/
    private bool IsProtocExist(string filepath,string fileName)
    {
        int i = fileName.LastIndexOf(".");
        string CFileHeaderName = fileName.Substring(0, i) + ".pb.h";
        string CFileSourceName = fileName.Substring(0, i) + ".pb.cc";
        string CFileHeaderFilePath = filepath + CFileHeaderName;
        string CFileSourceFilePath = filepath + CFileSourceName;
        //System.Console.WriteLine("CFileHeaderFilePath========== ####:" + CFileHeaderFilePath);
        //System.Console.WriteLine("CFileSourceFilePath========== ####:" + CFileSourceFilePath);

        return File.Exists(CFileHeaderFilePath) && File.Exists(CFileSourceFilePath);

    }

    /** return false: 不需要生成，return true: 需要重新生成*/
    private bool CheckProto(ProtobufConfig PBConfig, string filepath, string fileName)
    {
        if (PBConfig == null)
            return true;

        string pbMd5 = ProtobufConfig.HashMd5(filepath);
        string protoMd5 = PBConfig.GetValue("MD5Hash", fileName);
        if (pbMd5 == protoMd5)
        {
            return false;
        }
        else
        {
            PBConfig.AddValue("MD5Hash", fileName, pbMd5);
        }

        return true;    
    }

    private void GenerateProto()
    {
        ProtobufConfig ReadProtoVersionFile = ProtobufConfig.MakeInstance(ProtoPath, "version.config");

        string arg = "--proto_path=" + ProtoPath + " --cpp_out=dllexport_decl=PROTOBUF_API:" + SourcePath;
        string[] filenames = System.IO.Directory.GetFiles(ProtoPath, "*.proto");// GetFileSystemEntries(ProtoPath);
        string ExpandArg = "";
        foreach (string file in filenames)// 遍历所有的文件
        {
            int i = file.LastIndexOf("/");
            string fileName = file.Substring(i + 1);

            bool bCheck = CheckProto(ReadProtoVersionFile, file, fileName);
            bool bExisted = IsProtocExist(SourcePath, fileName);

            if (bCheck || !bExisted)
            {
                ExpandArg = ExpandArg + " " + fileName;
            }
        }

        ReadProtoVersionFile.Write();

        if (ExpandArg.Length > 0 )
        {
            arg = arg + ExpandArg;

            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = ProtocExe;
            process.StartInfo.Arguments = arg;
            process.Start();
        }
  
    }

    public ProtobufEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        //System.Console.WriteLine("ModuleDirectory========== ####:" + ModuleDirectory);

        if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
        {
            GenerateProto();
        }
            
    }
}
