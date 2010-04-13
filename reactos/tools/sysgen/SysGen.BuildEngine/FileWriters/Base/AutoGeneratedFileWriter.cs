using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

using SysGen.RBuild.Framework;

namespace SysGen.BuildEngine.Framework
{
    public abstract class AutoGeneratedFileWriter : StreamWriter
    {
        RBuildProject m_Project = null;
        RBuildModule m_Module = null;

        public AutoGeneratedFileWriter(RBuildProject project, string file)
            : base(file)
        {
            m_Project = project;
        }

        public AutoGeneratedFileWriter(RBuildModule module, string file)
            : base(file)
        {
            m_Module = module;
        }

        protected RBuildProject Project
        {
            get { return m_Project; }
        }

        protected RBuildModule Module
        {
            get { return m_Module; }
        }

        public abstract void WriteFile();
    }
}