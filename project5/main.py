import subprocess, os, re, zipfile
import argparse
from lxml import etree
import smtplib,json
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

parser = argparse.ArgumentParser(description="This is project number five for CSC344")
parser.add_argument("-p", "--path", help="this is the path to the directory", required=True)
parser.add_argument("-e", "--email", help="this the target email", required=True)
args = parser.parse_args()
path = args.path


# possible regrex for string (?<=\").*(?=\") better string? \"\s*.*?\s*\"
# possible #for comments (\#)+.*(?=\s)|(\#)+.*
# left side of assignment (\w)+. (?=\=)
# variables in paraenthese <=\()\s*([\w+\s*,]+)\s*(?=\))
# c comments (\//.*)|(/\\*.*?\\*/)
# clojure comments (\;.*)

# need to change to handle individual subdirectories
def get_ident(path):
    setty = set()
    # print(path, "get ident")
    for name in os.listdir(path):
        with open((path + name), 'r') as file:
            for line in file:
                # think this was to handle c style includes from python comments
                if '.c' in name:
                    pass
                else:
                    line = re.sub("(\#)+.*(?=\s)|(\#)+.*", "", line)
                line = re.sub("\"\s*.*?\s*\"", "", line)
                line = re.sub("(\%)+.*(?=\s)|(\%)+.*", "", line)
                line = re.sub("(\//.*)|(/\\*.*?\\*/)", "", line)
                line = re.sub("(\;.*)", "", line)
                line = line.rstrip('\r\n')
                line = re.findall("(\s*[a-zA-Z]*\s*)", line)

                line = list(filter(None, line))
                print(line,"final filter    ")

                for word in line:
                    word = word.strip()
                    word = word.rstrip('\r\n')
                    if bool(word) is False:
                        continue
                    word = re.sub("\s+", "", word)
                    word = word.rstrip('\n')
                    # print(word,"newline?")
                    if setty.__contains__(word) is False:
                        # print("-----", word)
                        setty.add(word)

    return setty


def zip_files(path):
    zipf = zipfile.ZipFile('Python.zip', 'w', zipfile.ZIP_DEFLATED)
    # path = "/home/doge/csc344/"
    for dirc in os.listdir(path):
        # print(dirc,"zip file dic")
        # print("dirc+patj",path+dirc+'/')
        if os.path.isdir((path + dirc + '/')):
            # print("zz")
            for name in os.listdir(path + '/' + dirc):
                # print("names:", name)
                zipf.write(path + '/' + dirc + '/' + name, "/zz/" + dirc + '/' + name)
        else:
            print("....")
            zipf.write(path + '/' + dirc, "/zz/" + dirc)
    zipf.close()


def email_file(user_email):
    server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
    user = ""
    passz = ""
    server.login(user, passz)
    filename = "Python.zip"
    attach = open(filename, "rb")
    msg = MIMEMultipart()
    msg['From'] = user
    msg['To'] = user_email
    msg.attach(MIMEText("hell ther e matewfsf"))
    part = MIMEApplication(attach.read(), Name="Python.zip")
    msg.attach(part)
    part.add_header('Content-Disposition', "attachment; filename= %s" % filename)
    server.sendmail(user, user_email, msg.as_string())
    server.close()


def get_wc(path):
    # gets wc for all files in current dir
    results = []
    if os.path.isdir(path) is True:
        for file in os.listdir(path):
            p = subprocess.Popen(["wc", "-l", path + file], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            res = p.communicate()[0].decode('UTF-8')
            amt = None
            grp = re.search("((\d+)(?=\s*))", str(res))
            if grp:
                amt = grp.group()
            results.append((file, amt))
        return results


def create_xml(path,folder, key_words, wc, ):
    header = b"""<?xml version="1.0" encoding="utf-8"?><?xml-stylesheet type='text/xsl' href='../project5/ref.xsl'?>"""
    page = etree.Element('html')
    doc = etree.ElementTree(page)
    headE = etree.SubElement(page, 'head')
    file_info = etree.SubElement(page, 'file_info')
    for word in wc:

        each_file = etree.SubElement(file_info, "indy", href= word[0])
        word_count = etree.SubElement(each_file, 'word_count')
        word_count.text = word[1]
        file_name = etree.SubElement(each_file, "file_path")
        file_name.text =folder+ word[0]
    for word in key_words:
        werwe = etree.SubElement(headE, 'identifier')
        tmp = etree.SubElement(werwe, "parsed")
        tmp.text = str(word)
    with open(path + 'homemade.xml', 'wb') as file:
        file.write(header)
        doc.write(file, xml_declaration=False, encoding='utf-8', method='xml')

def create_json(path,folder,key_words,line_count):
    results = dict()
    count = 0
    print(line_count)
    for inst in line_count:
        print(inst)
        results[count] ={}
        results[count]['path'] =folder +'/'+inst[0]
        results[count]['count'] = inst[1]
        results[count]['link'] = inst[0]
        count +=1
    results[0]["word"]= list(key_words)
    with open(path+'dumppy.json',"w") as file:
        file.write(json.dumps(results))
    with open(path+'data.js',"w") as file:
        file.write("var "+folder+"="+"{getData: function(){ var  DATA="+"'"+json.dumps(results)+"';"+"return DATA;}}")
def friedman(path):
    # call xml and wc func
    for folder in os.listdir(path):
        print(folder, "folder current in ")
        if os.path.isdir(path + folder) is True:
            line_count = get_wc(path + folder + '/')
            resp = get_ident(path + folder + '/')
            create_xml(path + folder + '/',folder, resp, line_count)
            create_json(path + folder + '/',folder, resp, line_count)
        else:
            print("else")
            continue

    zip_files(path)

    #email_file(args.email)


friedman(args.path)
